#include <TFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <iostream>
#include <fstream>  // For file input
#include <string>
#include <ctime>

void PileupHistogram2024Summer24_v1() {
    TChain chain("Events");

    // Read the list of ROOT files from a text file
    std::ifstream fileList("input_files/mcFiles_Summer24MG.txt");
    if (!fileList.is_open()) {
        std::cerr << "Error: Could not open input_files/mcFiles_Summer24MG.txt" << std::endl;
        return;
    }

    // Add each file in the list to the TChain
    std::string line;
    while (std::getline(fileList, line)) {
        chain.Add(line.c_str());
    }
    fileList.close();

    // Disable all branches by default
    chain.SetBranchStatus("*", 0);

    // Enable the "Pileup_nTrueInt" and "genWeight" branches
    chain.SetBranchStatus("Pileup_nTrueInt", 1);
    chain.SetBranchStatus("genWeight", 1);

    // Create a histogram to hold the pileup distribution
    TH1D *MCpileup = new TH1D("pileup", "PUProfile", 120, 0, 120);

    // Define variables to hold the values of Pileup_nTrueInt and genWeight
    Float_t Pileup_nTrueInt;
    Float_t genWeight;

    // Set the branch address to connect the variables with the branches in the tree
    chain.SetBranchAddress("Pileup_nTrueInt", &Pileup_nTrueInt);
    chain.SetBranchAddress("genWeight", &genWeight);

    // Loop over all events and fill the histogram with the weight applied
    Long64_t nEntries = chain.GetEntries();

    std::time_t startTime = std::time(nullptr);
    for (Long64_t i = 0; i < nEntries; i++) {
        chain.GetEntry(i);
        MCpileup->Fill(Pileup_nTrueInt, genWeight);  // Use weighted fill

        // Print progress every 1,000,000 events
        if (i % 1000000 == 0) {
            // Calculate elapsed time
            std::time_t currentTime = std::time(nullptr);
            double elapsedTime = std::difftime(currentTime, startTime);

            // Estimate remaining time
            double averageTimePerEvent = elapsedTime / (i + 1);
            double remainingEvents = nEntries - i;
            double estimatedTimeRemaining = averageTimePerEvent * remainingEvents;

            // Print progress and estimated time
            std::cout << "Processing event " << i << " of " << nEntries
                      << ". Elapsed time: " << elapsedTime << " seconds"
                      << ". Estimated time remaining: " << estimatedTimeRemaining << " seconds." << std::endl;
        }
    }

    // Specify where to save the output ROOT file
    TFile outFile("luminosityscripts/PUWeights/Summer24MG_PUProfile_v1.root", "RECREATE");
    MCpileup->Write();
    outFile.Close();

    // Clean up
    delete MCpileup;

    std::cout << "Pileup histogram created and saved to luminosityscripts/PUWeights/Summer24MG_PUProfile_v1.root" << std::endl;
}

