#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <chrono>
#include "TFile.h"
#include "TTree.h"
#include "nlohmann/json.hpp"

// Function to read the file list
std::vector<std::string> readFileList(const std::string &filePath) {
    std::vector<std::string> fileList;
    std::ifstream infile(filePath);
    std::string line;
    while (std::getline(infile, line)) {
        fileList.push_back(line);
    }
    return fileList;
}

// Function to extract run and lumi blocks from a ROOT file
std::map<int, std::set<int>> extractRunLumiBlocks(const std::string &rootFilePath) {
    std::map<int, std::set<int>> runLumiBlocks;
    TFile *file = TFile::Open(rootFilePath.c_str());
    
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file " << rootFilePath << std::endl;
        return runLumiBlocks;
    }
    
    TTree *tree = (TTree*)file->Get("Events");
    if (!tree) {
        std::cerr << "No 'Events' tree in " << rootFilePath << std::endl;
        return runLumiBlocks;
    }

    UInt_t run;
    UInt_t lumi;
    tree->SetBranchAddress("run", &run);
    tree->SetBranchAddress("luminosityBlock", &lumi);

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        runLumiBlocks[run].insert(lumi);
    }

    file->Close();
    return runLumiBlocks;
}

// Function to convert sets of luminosity blocks to ranges
std::vector<std::vector<int>> convertToRanges(const std::set<int> &lumiBlocks) {
    std::vector<std::vector<int>> ranges;
    if (lumiBlocks.empty()) return ranges;

    auto it = lumiBlocks.begin();
    int start = *it;
    int end = *it;

    for (++it; it != lumiBlocks.end(); ++it) {
        if (*it == end + 1) {
            end = *it;
        } else {
            ranges.push_back({start, end});
            start = *it;
            end = *it;
        }
    }
    ranges.push_back({start, end});
    return ranges;
}

// Main function
int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file_list_path> <output_json_path>" << std::endl;
        return 1;
    }

    std::string fileListPath = argv[1];
    std::string outputJsonPath = argv[2];

    std::vector<std::string> fileList = readFileList(fileListPath);
    std::map<int, std::set<int>> combinedData;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < fileList.size(); ++i) {
        std::cout << "Processing file " << (i + 1) << "/" << fileList.size() << ": " << fileList[i] << std::endl;
        std::map<int, std::set<int>> runLumiBlocks = extractRunLumiBlocks(fileList[i]);
        for (const auto &pair : runLumiBlocks) {
            combinedData[pair.first].insert(pair.second.begin(), pair.second.end());
        }
    }

    nlohmann::json combinedDataRanges;
    for (const auto &pair : combinedData) {
        combinedDataRanges[std::to_string(pair.first)] = convertToRanges(pair.second);
    }

    std::ofstream outputFile(outputJsonPath);
    outputFile << combinedDataRanges.dump(4);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    std::cout << "Processing completed in " << elapsed.count() << " seconds." << std::endl;

    return 0;
}

