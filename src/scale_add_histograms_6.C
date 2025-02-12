#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <iostream>
#include <vector>
#include <map>

void scale_add_histograms_6() {
    std::map<std::string, std::map<std::string, double>> luminosities = {
        {"2024F", {
	    {"HLT_ZeroBias", 0.000053563},
	    {"HLT_PFJet40", 5.3573000000000004e-05},
	    {"HLT_PFJet60", 0.000401718},
            {"HLT_PFJet80", 0.001606878},
            {"HLT_PFJet140", 0.018495671999999994},
            {"HLT_PFJet200", 0.079267192},
            {"HLT_PFJet260", 0.21674622799999993},
            {"HLT_PFJet320", 0.8669849109999997},
            {"HLT_PFJet400", 1.7339698199999996},
            {"HLT_PFJet450", 3.4679396439999994},
            {"HLT_PFJet500", 27.743517185999995}
        }},
        {"2024G", {
            {"HLT_ZeroBias", 0.000071646},
            {"HLT_PFJet40", 7.164999999999998e-05},
            {"HLT_PFJet60", 0.0005373320000000002},
            {"HLT_PFJet80", 0.0021237950000000004},
            {"HLT_PFJet140", 0.02514881},
            {"HLT_PFJet200", 0.107780636},
            {"HLT_PFJet260", 0.29471267999999984},
            {"HLT_PFJet320", 1.178850712},
            {"HLT_PFJet400", 2.3577014230000004},
            {"HLT_PFJet450", 4.715402843000001},
            {"HLT_PFJet500", 37.723222752}
        }},
        {"2024H", {
            {"HLT_ZeroBias", 0.000010331},
            {"HLT_PFJet40", 1.033e-05},
            {"HLT_PFJet60", 7.7481e-05},
            {"HLT_PFJet80", 0.000243364},
            {"HLT_PFJet140", 0.003626337},
            {"HLT_PFJet200", 0.015541446},
            {"HLT_PFJet260", 0.04249614},
            {"HLT_PFJet320", 0.16998455999999998},
            {"HLT_PFJet400", 0.339969123},
            {"HLT_PFJet450", 0.6799382450000001},
            {"HLT_PFJet500", 5.439505944}
        }},
        {"2024I", {
            {"HLT_ZeroBias", 0.000021684},
            {"HLT_PFJet40", 2.1661e-05},
            {"HLT_PFJet60", 0.000162437},
            {"HLT_PFJet80", 0.0005095310000000001},
            {"HLT_PFJet140", 0.007602628000000003},
            {"HLT_PFJet200", 0.032582683},
            {"HLT_PFJet260", 0.08909327500000001},
            {"HLT_PFJet320", 0.356373093},
            {"HLT_PFJet400", 0.7127461869999999},
            {"HLT_PFJet450", 1.4254923760000002},
            {"HLT_PFJet500", 11.403938998000001}
        }}
    };
    
    TH1D *hSumRec = nullptr;
    TH2D *hSumCov = nullptr;

    for (const auto &eraEntry : luminosities) {
        std::string era = eraEntry.first;
        std::string inputFilePath = "/media/storage/nestorma/dijet/rootfiles/v115_2024_v2/jmenano_data_out_" + era + "_JME_v115_2024_v2.root";
        TFile *inputFile = TFile::Open(inputFilePath.c_str(), "READ");

        if (!inputFile || inputFile->IsZombie()) {
            std::cerr << "Error: Unable to open " << inputFilePath << std::endl;
            continue;
        }

        for (const auto &triggerEntry : eraEntry.second) {
            std::string trigger = triggerEntry.first;
            double luminosity = triggerEntry.second;

            std::string histPathRec = trigger + "/Incjet/Unfolding/rec";
            TH1D *hTempRec = dynamic_cast<TH1D*>(inputFile->Get(histPathRec.c_str()));

            if (hTempRec) {
                hTempRec->SetDirectory(0); // Prevent ROOT from auto-deleting
                hTempRec->Scale(1.0 / luminosity);

                if (!hSumRec) {
                    hSumRec = (TH1D*)hTempRec->Clone();
                    hSumRec->Reset();
                    hSumRec->Add(hTempRec);
                } else {
                    hSumRec->Add(hTempRec);
                }
            } else {
                std::cerr << "Warning: Histogram " << histPathRec << " not found! Skipping." << std::endl;
            }

            std::string histPathCov = trigger + "/Incjet/Unfolding/cov";
            TH2D *hTempCov = dynamic_cast<TH2D*>(inputFile->Get(histPathCov.c_str()));

            if (hTempCov) {
                hTempCov->SetDirectory(0); // Prevent ROOT from auto-deleting
                hTempCov->Scale(1.0 / luminosity);

                if (!hSumCov) {
                    hSumCov = (TH2D*)hTempCov->Clone();
                    hSumCov->Reset();
                    hSumCov->Add(hTempCov);
                } else {
                    hSumCov->Add(hTempCov);
                }
            } else {
                std::cerr << "Warning: Histogram " << histPathCov << " not found! Skipping." << std::endl;
            }
        }

    }

    TFile *outputFile = TFile::Open("/media/storage/nestorma/dijet/rootfiles/v115_2024_v2/Data_unf.root", "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Unable to create output file!" << std::endl;
        return;
    }

    TDirectory *nominalDir = outputFile->mkdir("nominal");
    nominalDir->cd(); 

    if (hSumRec) {
        hSumRec->Write("rec"); 
    } else {
        std::cerr << "Error: No valid rec histograms found!" << std::endl;
    }

    if (hSumCov) {
        hSumCov->Write("cov");
    } else {
        std::cerr << "Error: No valid cov histograms found!" << std::endl;
    }
    std::cout << "Output saved to /media/storage/nestorma/dijet/rootfiles/v115_2024_v2/Data_unf.root" << std::endl;
}

