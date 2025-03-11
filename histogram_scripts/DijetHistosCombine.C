// Purpose: Combine output from DijetHistosFill for different triggers
// Author:  Mikko Voutilainen, 17-Mar-2022
#include "TFile.h"
#include "TKey.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile2D.h"
#include "TH3D.h"
#include <iostream>

int debug = 0; // 1=trg, 2=dir, 3=all
string version = "v118";
void loopOverDirectories(TDirectory *dir, TDirectory *outdir,
			 string trg, string folder);
//void mergeDijet(TDirectory *dir, TDirectory *dout);
bool copyBin(string trg, string folder, string histo, double pt, double eta);

void DijetHistosCombines(string file = "rootfiles/jmenano_data_out.root");

void DijetHistosCombine() {

  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022C_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022D_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022CD_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022E_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2022C_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2022D_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2022E_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2022F_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2022G_nib1_JME_"+version+".root");
  // Nestor run
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022E_"+version+"_22Sep2023.root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022F_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022G_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2022FG_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2023BCv123_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2023Cv4_JME_"+version+".root");
  //DijetHistosCombines("../rootfiles/jmenano_data_out_2023D_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Bv1_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv1_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv2_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv3_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv4_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv4_nib2_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Dv1_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Dv2_nib1_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2022C_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2022D_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2022CD_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2022E_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2022F_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2022G_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024B_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024C_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024D_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024BCD_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024E_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024Ev1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024Ev2_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024F_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024F_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024G_nib2_JME_v116.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024G_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024GH_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024H_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024I_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024BR_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024CR_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024CS_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024CT_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024F_TeVJet_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024F_JetHT_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024H_Skim_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024B_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024C_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024D_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024Ev1_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024Ev2_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024F_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024F_nib2_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024F_nib3_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024G_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024G_nib2_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024H_nib1_JME_"+version+".root");
  DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024H_nib1_"+version+".root");
  DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024H_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024Iv1_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024Iv2_nib1_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024I_nib1_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2024D_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2024BC_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv123_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023Cv4_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2023D_JME_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_data_out_2024C_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024_skim_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MCFlat_Sv9_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MCFlat_Sv10_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2022CD_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024B_ECALv2_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024B_ECALv1_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"_2024/jmenano_data_out_2024F_ECAL2_"+version+"_2024.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024C_Rp_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024D_Rp_JME_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_data_out_2024E_Rp_JME_"+version+".root");

  // Really slow on this after all the others, rerun separately (then sec)
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer22MG_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer22EEMG_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer22MG_full_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer22EEMG_full_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer23MGBPix_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer23MG_Cv123_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer23MG_Cv4_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer23MGBPix_"+version+".root");
  //DijetHistosCombines("/Users/nestorma/Documents/Helsinki/dijet/rootfiles/v35a/2022E/jmenano_mc_out_Summer22MG_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MCFlat_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MCFlat_Sv9_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MCFlat_Sv10_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MG_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"B.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"C.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"D.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"E.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"F.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"G.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+"BCD.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v109_JRSF_2024BCD.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v109_JRSF_2024E.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_JRSF_v110_1_2024BCD.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_JRSF_v110_1_2024E.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_JRSF_v110_1_2024F.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_JRSF_v110_1_2024G.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024BCD_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024E_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024F_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024G_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024GH_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024GHv2_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024H_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024Hv2_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_v112_2024I_JRSF.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Winter24MGV14_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer24MG_"+version+".root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer24MG_v116_2024F_nib2.root");
  //DijetHistosCombines("/media/storage/nestorma/dijet/rootfiles/"+version+"/jmenano_mc_out_Summer24MG_v116_2024Ev2_nib1.root");

  //DijetHistosCombines("rootfiles/jmenano_mc_out_v23ul16flat.root");
  //DijetHistosCombines("rootfiles/jmenano_mc_out_v23ul16mg.root");

  // This one is taking a while. Why? CPU ~100%, mem up to >3 GB
  //DijetHistosCombines("haddfiles/jmenano_data_out_Run2_v26c.root");
  //DijetHistosCombines("haddfiles/jmenano_mc_out_Run2_v26.root");
  /*
  // After JER SF for MC
  DijetHistosCombines("rootfiles/jmenano_mc_out_UL2016APVMG_v27.root");
  DijetHistosCombines("rootfiles/jmenano_mc_out_UL2016MG_v27.root");
  DijetHistosCombines("rootfiles/jmenano_mc_out_UL2017MG_v27.root");
  DijetHistosCombines("rootfiles/jmenano_mc_out_UL2018MG_v27.root");
  DijetHistosCombines("haddfiles/jmenano_mc_out_Run2_v27.root");
  */

  // New Run3 files from Iita and Mikael
  // Iita_20230814/*_v1.root -> Iita_20230824_jetveto/*_JME_v1.root
  /*
  // 2022
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022C_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022D_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022E_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022F_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022G_JME_v1.root");
  // 2023
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2023B_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2023Cv123_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2023BCv123_JME_v1.root");
  DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2023Cv4_JME_v1.root");
  */
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2023D_JME_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_mc_out_Summer22_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_mc_out_Summer22EE_v1.root");
  // Main combos (after checking stability for L2Res)
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022CD_JME_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022FG_JME_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2022EFG_JME_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2023Cv4D_JME_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/nano_data_out_2022BCv123_JME_v1.root");
  //
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_data_out_2223_JME_v1.root");
  //DijetHistosCombines("../jecsys3/rootfiles/Iita_20230824_jetveto/jmenano_mc_out_Summer22Both_v1.root");
  
} // DijetHistosCombine

void DijetHistosCombines(string file) {

  cout << "DijetHistosCombines(\"" << file << "\")" << endl;
  TDirectory *curdir = gDirectory;
  
  // Open input and output files
  TFile *fin = new TFile(file.c_str(),"READ");
  assert(fin && !fin->IsZombie());
  
  TString t = file.c_str();
  t.ReplaceAll("_out","_cmb");
  string file2 = t.Data();
  assert(file2!=file);
  cout << "                 => \"" << file2 << endl;

  TFile *fout = new TFile(file2.c_str(),"RECREATE");
  assert(fout && !fout->IsZombie());

  
  // Retrieve listing of available triggers from input file
  TH1D *htrg = (TH1D*)fin->Get("htrg");
  assert(htrg);

  // Enter first folder and prepare folders+histograms for output file
  fin->cd(htrg->GetXaxis()->GetBinLabel(1));
  //fin->cd("HLT_PFJet450");
  TDirectory *dir = gDirectory;
  if (debug>0) cout << "Initialize with " << dir->GetName() << endl << flush;
  loopOverDirectories(dir,fout,"none","");
  
  // Then copy stuff over
  for (int i = 1; i != htrg->GetNbinsX()+1; ++i) {
    
    string trg = htrg->GetXaxis()->GetBinLabel(i);
    if (!TString(trg.c_str()).Contains("HLT_")) continue;
    fin->cd(trg.c_str());
    dir = gDirectory;
    if (debug>0) cout << "Process " << trg << " in "
		      << dir->GetName() << endl << flush;
    loopOverDirectories(dir,fout,trg.c_str(),"");
  }

  //mergeDijet(fin, fout);

  fout->Write();
} // DijetHistosCombines


void loopOverDirectories(TDirectory *dir, TDirectory *outdir,
			 string trg, string folder) {

  TIter next(dir->GetListOfKeys());
  while (TKey *key = (TKey*)next()) {

    // Recurse directory structure
    if (string(key->GetClassName())=="TDirectoryFile") {
      if (debug>1) cout << key->GetName() << "->";
      TDirectory *subdir = (TDirectory*)key->ReadObj();

      if (!outdir->FindObject(subdir->GetName()))
	outdir->mkdir(subdir->GetName());
      outdir->cd(subdir->GetName());
      TDirectory *suboutdir = gDirectory;
      //cout << "subdir " << subdir << "suboutdir " << suboutdir << "and trg " << trg << "in folder " << folder << endl << flush;

      loopOverDirectories(subdir, suboutdir,
			  trg=="" ? key->GetName() : trg,
			  trg=="" ? "" : (folder=="" ? key->GetName():folder));
    } 
    // Create histograms, if not yet there
    else {

      TObject *obj = key->ReadObj();
      
      if (obj->InheritsFrom("TProfile2D")) {
	TProfile2D *p2 = (TProfile2D*)obj;
	
	/*
	// Collapse to TH2D until can figure out how to copy bins of TProfile2D
	// This unfortunately makes rebinning later trickier
	TH2D *h2o = (TH2D*)outdir->FindObject(key->GetName());
	if (!h2o) {
	  outdir->cd();
	  h2o = p2->ProjectionXY(key->GetName());
	  h2o->Reset();
	}

	for (int binx = 1; binx != p2->GetNbinsX()+1; ++binx) {
	  for (int biny = 1; biny != p2->GetNbinsY()+1; ++biny) {
	    int ibin = p2->GetBin(binx, biny);
	    if (copyBin(trg, folder, key->GetName(),
			h2o->GetYaxis()->GetBinCenter(biny),
			h2o->GetXaxis()->GetBinCenter(binx), 0.)) {
	      if (folder=="Jetveto") {
		h2o->SetBinContent(ibin, h2o->GetBinContent(ibin)+
				   p2->GetBinContent(ibin));
		h2o->SetBinError(ibin, sqrt(pow(h2o->GetBinError(ibin),2)+
					    pow(p2->GetBinError(ibin),2)));
	      }
	      else {
		assert(h2o->GetBinContent(ibin)==0);
		h2o->SetBinContent(ibin, p2->GetBinContent(ibin));
		h2o->SetBinError(ibin, p2->GetBinError(ibin));
	      }
	    }
	  } // for biny
	} // for biny
	*/

	TProfile2D *p2o = (TProfile2D*)outdir->FindObject(key->GetName());
	if (!p2o) {
	  outdir->cd();
	  p2o = (TProfile2D*)p2->Clone(key->GetName());
	  p2o->Reset();
	}

	// profile keeps track of sumw, sumwz, sumwz2, sumw2
	// sumw=fArray, sumwz=fBinEntries.fArray, 
	// sumwz2 = fBinSumw2.fArray, sumw2 = fSum2.fArray

	// GetBinContent = sumwz/sumw

	// https://root-forum.cern.ch/t/copy-entries-of-tprofile/11828
	for (int binx = 1; binx != p2->GetNbinsX()+1; ++binx) {
	  for (int biny = 1; biny != p2->GetNbinsY()+1; ++biny) {
	    int ibin = p2->GetBin(binx, biny);
	    if (copyBin(trg, folder, key->GetName(),
			p2o->GetYaxis()->GetBinCenter(biny),
			p2o->GetXaxis()->GetBinCenter(binx))) {
	      if (folder=="Jetveto") {
	
		p2o->SetEntries(p2o->GetEntries()+p2->GetEntries());
		(*p2o)[ibin] = (*p2)[ibin] + (*p2o)[ibin];
		(*p2o->GetSumw2())[ibin] = (*p2->GetSumw2())[ibin] +
		  (*p2o->GetSumw2())[ibin];
		p2o->SetBinEntries(ibin, p2->GetBinEntries(ibin) +
				   p2o->GetBinEntries(ibin));
		// copy (if needed) bin sum of weight square
		if ( p2->GetBinSumw2()->fN > ibin ) { 
		  //p2o->Sumw2();
		  (*p2o->GetBinSumw2())[ibin] = (*p2->GetBinSumw2())[ibin] +
		    (*p2o->GetBinSumw2())[ibin];
		}
	      }	// Jetveto
	      else {
		p2o->SetEntries(p2o->GetEntries()+p2->GetEntries());
		(*p2o)[ibin] = (*p2)[ibin]; // copy bin y values
		/*
		if (p2->GetSumw2()->fN > ibin) {
                   (*p2o->GetSumw2())[ibin] = (*p2->GetSumw2())[ibin];
                }
		else{
                   cout << "GetSumw2 is not greater than ibin " <<  endl << flush;
		}
		*/
		(*p2o->GetSumw2())[ibin] = (*p2->GetSumw2())[ibin]; // copy y*y
		p2o->SetBinEntries(ibin, p2->GetBinEntries(ibin));  // entries
		// copy (if needed) bin sum of weight square
		if ( p2->GetBinSumw2()->fN > ibin ) { 
		  //p2o->Sumw2();
		  (*p2o->GetBinSumw2())[ibin] = (*p2->GetBinSumw2())[ibin];   
		}
	      } // !Jetveto
	    } // copyBin
	  } // for biny
	} // for biny

      } // TProfile2D

      else if (obj->InheritsFrom("TH3D")) {
        TH3D *h3 = (TH3D*)obj;
        TH3D *h3o = (TH3D*)outdir->FindObject(key->GetName());
        if (!h3o) {
          outdir->cd();
          h3o = (TH3D*)h3->Clone(key->GetName());
          h3o->Reset();
        }
        for (int binx = 1; binx != h3->GetNbinsX()+1; ++binx) {
          for (int biny = 1; biny != h3->GetNbinsY()+1; ++biny) {
            for (int binz = 1; binz != h3->GetNbinsZ()+1; ++binz) {
              int ibin = h3->GetBin(binx, biny, binz);
              string hist = key->GetName();
	      copyBin(trg, folder, key->GetName(),
		      h3o->GetYaxis()->GetBinCenter(biny),
		      h3o->GetXaxis()->GetBinCenter(binx));
              if (folder=="Dijet2" && (hist=="h3m0"||hist=="h3m2")) {
                h3o->SetBinContent(ibin, h3o->GetBinContent(ibin)+
                                   h3->GetBinContent(ibin));
                h3o->SetBinError(ibin, sqrt(pow(h3o->GetBinError(ibin),2)+
					    pow(h3->GetBinError(ibin),2)));
              }
              else if (folder=="Jetveto" &&(hist=="Asymm"||hist=="h3asymm")) {
                h3o->SetBinContent(ibin, h3o->GetBinContent(ibin)+
                                   h3->GetBinContent(ibin));
                h3o->SetBinError(ibin, sqrt(pow(h3o->GetBinError(ibin),2)+
                                            pow(h3->GetBinError(ibin),2)));
              }
	      else {
		h3o->SetBinContent(ibin, h3->GetBinContent(ibin));
		h3o->SetBinError(ibin, h3->GetBinError(ibin));
	      }
            } // for binz
          } // for biny
        } // for binx
      } // TH3D

      else if (obj->InheritsFrom("TH2D")) {
	TH2D *h2 = (TH2D*)obj;
	TH2D *h2o = (TH2D*)outdir->FindObject(key->GetName());
	if (!h2o) {
	  outdir->cd();
	  h2o = (TH2D*)h2->Clone(key->GetName());
	  h2o->Reset();
	}
	for (int binx = 1; binx != h2->GetNbinsX()+1; ++binx) {
	  for (int biny = 1; biny != h2->GetNbinsY()+1; ++biny) {
	    int ibin = h2->GetBin(binx, biny);
	    string hist = key->GetName();
	    double cpBin = 
	      ((folder=="Multijet" && (hist=="h2recoila"||hist=="h2recoilm"||
				       hist=="h2recoill" ||hist=="h2recoilr")) ?
	       copyBin(trg, folder, key->GetName(),
		       h2o->GetXaxis()->GetBinCenter(binx), 0.) :
	       copyBin(trg, folder, key->GetName(),
		       h2o->GetYaxis()->GetBinCenter(biny),
		       h2o->GetXaxis()->GetBinCenter(binx)));
	    if (cpBin) {
	      if (folder=="Jetveto") {
		h2o->SetBinContent(ibin, h2o->GetBinContent(ibin)+
				   h2->GetBinContent(ibin));
		h2o->SetBinError(ibin, sqrt(pow(h2o->GetBinError(ibin),2)+
					    pow(h2->GetBinError(ibin),2)));
	      }
	      else {
		h2o->SetBinContent(ibin, h2->GetBinContent(ibin));
		h2o->SetBinError(ibin, h2->GetBinError(ibin));
	      }
	    }
	  } // for biny
	} // for binx
      } // TH2D

      else if (obj->InheritsFrom("TProfile")) {
          TProfile *p = (TProfile*)obj;

          TProfile *po = (TProfile*)outdir->FindObject(key->GetName());
          if (!po) {
              outdir->cd();
              po = (TProfile*)p->Clone(key->GetName());
              po->Reset();
	      //po->Sumw2(); // Nestor, Oct 24, 2024. Problems with 2024F_TeVJet.
          }

	  // https://root-forum.cern.ch/t/copy-entries-of-tprofile/11828
          for (int ibin = 1; ibin != p->GetNbinsX()+1; ++ibin) {
              if (folder == "JetsperRuns") {
                  // Directly copy for JetsperRuns
                  (*po)[ibin] = (*p)[ibin];  // copy bin y values
                  (*po->GetSumw2())[ibin] = (*p->GetSumw2())[ibin];  // copy bin y*y
                  po->SetBinEntries(ibin, p->GetBinEntries(ibin));  // copy entries

                  if (p->GetBinSumw2()->fN > ibin) {
                      (*po->GetBinSumw2())[ibin] = (*p->GetBinSumw2())[ibin];  // copy sumw2
                  }
              } else {
                  // For other folders, apply the copyBin condition
                  if (copyBin(trg, folder, key->GetName(), po->GetBinCenter(ibin), 0.)) {
                      (*po)[ibin] = (*p)[ibin];  // copy bin y values
                      (*po->GetSumw2())[ibin] = (*p->GetSumw2())[ibin];  // copy bin y*y
                      po->SetBinEntries(ibin, p->GetBinEntries(ibin));  // copy entries

                      if (p->GetBinSumw2()->fN > ibin) {
		      //if (p->GetBinSumw2()->fN > ibin && po->GetBinSumw2()->fN > ibin) {
                          (*po->GetBinSumw2())[ibin] = (*p->GetBinSumw2())[ibin];  // copy sumw2
                      }
                  }
              }
          } // for ibin
      } // TProfile

      else if (obj->InheritsFrom("TH1D")) {
	//cout << __LINE__ << ' ' << folder << endl;
	TH1D *h = (TH1D*)obj;
	TH1D *ho = (TH1D*)outdir->FindObject(key->GetName());
	if (!ho) {
	  outdir->cd();
	  ho = (TH1D*)h->Clone(key->GetName());
	  ho->Reset();
	}
	string hist = key->GetName();
	for (int ibin = 1; ibin != h->GetNbinsX()+1; ++ibin) {
	  int ieta(0);
	  if (folder=="Incjet") sscanf(key->GetName(),"hpt%d",&ieta);
	  if (folder=="JetsperRuns") {

            if (copyBin(trg, folder, key->GetName(),
                        //ho->GetBinCenter(ibin),0)) {
                        0.,0.)) {
              ho->SetBinContent(ibin, h->GetBinContent(ibin));
              ho->SetBinError(ibin, h->GetBinError(ibin));
            }
          }
          else {
            if (copyBin(trg, folder, key->GetName(),
                        ho->GetBinCenter(ibin),0.1*ieta)) {
              ho->SetBinContent(ibin, h->GetBinContent(ibin));
              ho->SetBinError(ibin, h->GetBinError(ibin));
            }
          }
	} // for ibin
      } // TH1D


      if (debug>2) cout << endl << "  " << key->GetName();
    }
  }
  if (debug>1) cout << endl;
} // loopOverDirectories

struct range {
  int ptmin;
  int ptmax;
  double absetamin;
  double absetamax;
};
std::map<std::string, struct range> md;
std::map<std::string, struct range> md2;
std::map<std::string, struct range> md2tc;
std::map<std::string, struct range> md2pf;
std::map<std::string, struct range> mj;
std::map<std::string, struct range> mi;

// Simple struct to hold the region boundaries for each trigger
struct TriggerBox {
    std::string triggerName;
    double ptMin;
    double ptMax;
    double absyMin;
    double absyMax;
};
// Inclusive jets pT bins in relevan trigger range
//28, 32, 37, 43, 49, 56, 64, 74, 84, 97, 114, 133, 153, 174, 196, 220, 245,
//272, 300, 330, 362, 395, 430, 468, 507, 548, 592, 638, 686, 737, 790, 846,
// Example: define a vector of TriggerBoxes for the inclusive-jet folder
std::vector<TriggerBox> incjetBoxes = {
  //{"HLT_ZeroBias", 0,   49,  0, 5.2},
  {"HLT_ZeroBias", 0,   56,  0.0, 0.5},
  {"HLT_ZeroBias", 0,   56,  0.5, 1.0},
  {"HLT_ZeroBias", 0,   64,  1.0, 1.5},
  {"HLT_ZeroBias", 0,   64,  1.5, 2.0},
  {"HLT_ZeroBias", 0,  114,  2.0, 2.5},
  {"HLT_ZeroBias", 0,   97,  2.5, 3.0},
  //{"HLT_PFJet40",  49,  84,  0, 3.0},
  {"HLT_PFJet40", 56,  84,  0.0, 0.5},
  {"HLT_PFJet40", 56,  84,  0.5, 1.0},
  {"HLT_PFJet40", 64,  84,  1.0, 1.5},
  {"HLT_PFJet40", 64,  84,  1.5, 2.0},
  {"HLT_PFJet40", 114, 153,  2.0, 2.5},
  {"HLT_PFJet40", 97,  114,  2.5, 3.0},  
  //{"HLT_PFJet60",  84,  114, 0, 3.0},
  {"HLT_PFJet60", 84,  114,  0.0, 0.5},
  {"HLT_PFJet60", 84,  114,  0.5, 1.0},
  {"HLT_PFJet60", 84,  114,  1.0, 1.5},
  {"HLT_PFJet60", 84,  114,  1.5, 2.0},
  {"HLT_PFJet60", 153, 174,  2.0, 2.5},
  {"HLT_PFJet60", 114, 133,  2.5, 3.0},  
  //{"HLT_PFJet80",  114, 196, 0, 3.0},
  {"HLT_PFJet80", 114, 196,  0.0, 0.5},
  {"HLT_PFJet80", 114, 196,  0.5, 1.0},
  {"HLT_PFJet80", 114, 196,  1.0, 1.5},
  {"HLT_PFJet80", 114, 196,  1.5, 2.0},
  {"HLT_PFJet80", 174, 196,  2.0, 2.5},
  {"HLT_PFJet80", 133, 196,  2.5, 3.0},  
  
  {"HLT_PFJet140", 196, 272, 0, 3.0},
  {"HLT_PFJet200", 272, 330, 0, 3.0},
  {"HLT_PFJet260", 330, 395, 0, 3.0},
  {"HLT_PFJet320", 395, 468, 0, 3.0},
  {"HLT_PFJet400", 468, 548, 0, 3.0},
  {"HLT_PFJet450", 548, 686, 0, 3.0},
  {"HLT_PFJet500", 686,7000, 0, 3.0},

  //{"HLT_ZeroBias", 0,   49,  0, 5.2}, 
  {"HLT_ZeroBias", 0,   64,  3.0, 3.5},
  {"HLT_ZeroBias", 0,   64,  3.5, 4.0},
  {"HLT_ZeroBias", 0,   64,  4.0, 4.5},
  {"HLT_ZeroBias", 0,   64,  4.5, 5.2},
  //{"HLT_PFJetFwd40",  49,  84,  3.0, 5.2},
  {"HLT_PFJetFwd40",  64,   97,  3.0, 3.5},
  {"HLT_PFJetFwd40",  64,   97,  3.5, 4.0},
  {"HLT_PFJetFwd40",  64,   97,  4.0, 4.5},
  {"HLT_PFJetFwd40",  64,   97,  4.5, 5.2},
  //{"HLT_PFJetFwd60",  84,  114, 3.0, 5.2},
  {"HLT_PFJetFwd60",  97,  220,  3.0, 3.5},
  {"HLT_PFJetFwd60",  97,  220,  3.5, 4.0},
  {"HLT_PFJetFwd60",  97,  220,  4.0, 4.5},
  {"HLT_PFJetFwd60",  97,  153,  4.5, 5.2},
  //{"HLT_PFJetFwd80",  114, 196, 3.0, 5.2},
  // Fwd80 somehow botched up, very bad turn-on at ~196 (last 133?)
  {"HLT_PFJetFwd80", 220,  220,  3.0, 3.5},
  {"HLT_PFJetFwd80", 220,  220,  3.5, 4.0},
  {"HLT_PFJetFwd80", 220,  220,  4.0, 4.5},
  {"HLT_PFJetFwd80", 153,  153,  4.5, 5.2},
  //{"HLT_PFJetFwd140", 196, 272, 3.0, 5.2},
  {"HLT_PFJetFwd140", 220,  272,  3.0, 3.5},
  {"HLT_PFJetFwd140", 220,  272,  3.5, 4.0},
  {"HLT_PFJetFwd140", 220,  272,  4.0, 4.5},
  {"HLT_PFJetFwd140", 153, 7000,  4.5, 5.2}, // E<150 for hpt50
  
  //{"HLT_PFJetFwd200", 272, 330, 3.0, 5.2},
  {"HLT_PFJetFwd200", 272, 330, 3.0, 4.0},
  {"HLT_PFJetFwd200", 272, 300, 4.0, 4.5},
  {"HLT_PFJetFwd200",7000,7000, 4.5, 5.2}, // 150-200 for hpt50
  //{"HLT_PFJetFwd260", 330, 395, 3.0, 5.2},
  {"HLT_PFJetFwd260", 330, 362, 3.0, 4.0},
  {"HLT_PFJetFwd260", 300,7000, 4.0, 4.5}, // 300 for hpt45
  {"HLT_PFJetFwd260",7000,7000, 4.5, 5.2}, // 150-200 for hpt50
  //{"HLT_PFJetFwd320", 395, 468, 3.0, 5.2},
  {"HLT_PFJetFwd320", 362, 430, 3.0, 4.0},
  {"HLT_PFJetFwd320",7000,7000, 4.0, 4.5},
  {"HLT_PFJetFwd320",7000,7000, 4.5, 5.2},
  //{"HLT_PFJetFwd400", 468, 7000, 3.0, 5.2},
  {"HLT_PFJetFwd400", 430,7000, 3.0, 4.0},
  {"HLT_PFJetFwd400",7000,7000, 4.0, 4.5},
  {"HLT_PFJetFwd400",7000,7000, 4.5, 5.2},
  //{"HLT_PFJetFwd400", 468, 548, 3.0, 5.2},
  //{"HLT_PFJetFwd450", 548, 686, 3.0, 5.2},
  //{"HLT_PFJetFwd500", 686,7000, 3.0, 5.2},
  /*
    {"HLT_PFJet40",   40.0,   60.0,   0.0,   0.5},
    {"HLT_PFJet40",   40.0,   60.0,   0.5,   1.0},
    {"HLT_PFJet500", 500.0, 1000.0,   0.0,   2.0},
    // ... Add as many boxes as you like
  */
};
// Utility to check if a bin center is in range
inline bool inRange(double val, double minVal, double maxVal) {
    return (val >= minVal) && (val < maxVal);
}

bool copyBin(string trg, string folder, string hist, double pt, double eta) {


  // Setup triggers only once
  if (md.find("HLT_ZeroBias")==md.end()) {

    double fwdeta = 3.0; // was 3.139; // was 2.853. 80% (100%) on negative (positive) side
    double fwdeta0 = 2.964;//2.853; // 40 and 260 up
    double fwdetad = 2.853;

    // Dijet thresholds
    md["HLT_ZeroBias"]      = range{15,  40,  0, 5.2};
    md["HLT_MC"]            = range{15,6500,  0, 5.2};
    
    md["HLT_DiPFJetAve40"]  = range{40,  85,  0, 5.2};
    md["HLT_DiPFJetAve60"]  = range{85,  100, 0, fwdeta};
    md["HLT_DiPFJetAve80"]  = range{100, 155, 0, fwdeta};
    md["HLT_DiPFJetAve140"] = range{155, 250, 0, fwdeta};
    md["HLT_DiPFJetAve200"] = range{250, 300, 0, fwdeta0}; // 210->250
    md["HLT_DiPFJetAve260"] = range{300, 400, 0, fwdeta0};
    md["HLT_DiPFJetAve320"] = range{400, 500, 0, fwdeta0};
    md["HLT_DiPFJetAve400"] = range{500, 600, 0, fwdeta0};
    md["HLT_DiPFJetAve500"] = range{600,3000, 0, fwdeta0};

    //JEC4Prompt
    
    md["HLT_DiPFJetAve60_HFJEC"]  = range{85,  100, fwdeta, 5.2};
    md["HLT_DiPFJetAve80_HFJEC"]  = range{100, 125, fwdeta, 5.2};
    md["HLT_DiPFJetAve100_HFJEC"] = range{125, 180, fwdeta, 5.2};
    md["HLT_DiPFJetAve160_HFJEC"] = range{180, 250, fwdeta, 5.2};
    md["HLT_DiPFJetAve220_HFJEC"] = range{250, 350, fwdeta0, 5.2};
    md["HLT_DiPFJetAve300_HFJEC"] = range{350,3000, fwdeta0, 5.2};
    

    // https://indico.cern.ch/event/1263476/contributions/5311425/attachments/2612023/4513129/L2Res+HDM-March15.pdf
    md2["HLT_ZeroBias"]      = range{15,  59,  0, 5.2};
    md2["HLT_MC"]            = range{15,6500,  0, 5.2};
    
    md2["HLT_DiPFJetAve40"]  = range{59,  86,  0, 5.2};
    md2["HLT_DiPFJetAve60"]  = range{86,  110, 0, fwdetad};
    md2["HLT_DiPFJetAve80"]  = range{110, 170, 0, fwdetad};
    md2["HLT_DiPFJetAve140"] = range{170, 236, 0, fwdetad};
    md2["HLT_DiPFJetAve200"] = range{236, 302, 0, fwdetad};
    md2["HLT_DiPFJetAve260"] = range{302, 373, 0, fwdetad};
    md2["HLT_DiPFJetAve320"] = range{373, 460, 0, fwdetad};
    md2["HLT_DiPFJetAve400"] = range{460, 575, 0, fwdetad};
    md2["HLT_DiPFJetAve500"] = range{575,6500, 0, fwdetad};
    
    md2["HLT_DiPFJetAve60_HFJEC"]  = range{86,  110, fwdetad, 5.2};
    md2["HLT_DiPFJetAve80_HFJEC"]  = range{110, 132, fwdetad, 5.2};
    md2["HLT_DiPFJetAve100_HFJEC"] = range{132, 204, fwdetad, 5.2};
    md2["HLT_DiPFJetAve160_HFJEC"] = range{204, 279, fwdetad, 5.2};
    md2["HLT_DiPFJetAve220_HFJEC"] = range{279, 373, fwdetad, 5.2};
    md2["HLT_DiPFJetAve300_HFJEC"] = range{373,3000, fwdetad, 5.2};

    md2pf["HLT_ZeroBias"] = range{15,  59,  0, 5.2};
    md2pf["HLT_MC"]       = range{15,6500,  0, 5.2};
    md2pf["HLT_PFJet40"]  = range{59,  86,  0, 5.2};
    md2pf["HLT_PFJet60"]  = range{86,  110, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet80"]  = range{110, 170, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet140"] = range{170, 236, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet200"] = range{236, 302, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet260"] = range{302, 373, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet320"] = range{373, 460, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet400"] = range{460, 575, 0, 5.2};//fwdetad};
    md2pf["HLT_PFJet500"] = range{575,6500, 0, 5.2};//fwdetad};
    
    md2pf["HLT_PFJetFwd40"]  = range{49,  84,  fwdetad, 5.2};   //Added to check HLT PFJetFwd. Nestor. April 25, 2024.
    md2pf["HLT_PFJetFwd60"]  = range{84,  114, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd80"]  = range{114, 196, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd140"] = range{196, 272, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd200"] = range{272, 330, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd260"] = range{330, 395, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd320"] = range{395, 468, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd400"] = range{468, 548, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd450"] = range{548, 686, fwdetad, 5.2};
    md2pf["HLT_PFJetFwd500"] = range{686,6500, fwdetad, 5.2};   //
    
    md2tc["HLT_ZeroBias"] = range{15,  59,  0, 5.2};
    md2tc["HLT_MC"]       = range{15,6500,  0, 5.2};
    md2tc["HLT_PFJet40"]  = range{59,  86,  0, 5.2};
    md2tc["HLT_PFJet60"]  = range{86,  110, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet80"]  = range{110, 170, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet140"] = range{170, 236, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet200"] = range{236, 302, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet260"] = range{302, 373, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet320"] = range{373, 460, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet400"] = range{460, 575, 0, 5.2};//fwdetad};
    md2tc["HLT_PFJet500"] = range{575,6500, 0, 5.2};//fwdetad};
    
    // Multijet or dijet tag/probe thresholds
    mj["HLT_PFJet40"]  = range{40,  85,  0, fwdeta0};
    mj["HLT_PFJet60"]  = range{85,  100, 0, fwdeta};
    mj["HLT_PFJet80"]  = range{100, 155, 0, fwdeta};
    mj["HLT_PFJet140"] = range{155, 210, 0, fwdeta};
    mj["HLT_PFJet200"] = range{210, 300, 0, fwdeta0};
    mj["HLT_PFJet260"] = range{300, 400, 0, fwdeta0};
    mj["HLT_PFJet320"] = range{400, 500, 0, fwdeta0};
    mj["HLT_PFJet400"] = range{500, 600, 0, fwdeta0};
    mj["HLT_PFJet450"] = range{500, 600, 0, fwdeta0};
    mj["HLT_PFJet500"] = range{600,3000, 0, fwdeta0};
    //mj["HLT_PFJet500"] = range{600, 700, 0, fwdeta0};
    //mj["HLT_PFJet550"] = range{700,3000, 0, fwdeta0};
    
    mj["HLT_PFJetFwd40"]  = range{40,  85,  fwdeta0, 5.2};
    mj["HLT_PFJetFwd60"]  = range{85,  100, fwdeta, 5.2};
    mj["HLT_PFJetFwd80"]  = range{100, 155, fwdeta, 5.2};
    mj["HLT_PFJetFwd140"] = range{155, 210, fwdeta, 5.2};
    mj["HLT_PFJetFwd200"] = range{210, 300, fwdeta0, 5.2};
    mj["HLT_PFJetFwd260"] = range{300, 400, fwdeta0, 5.2};
    mj["HLT_PFJetFwd320"] = range{400, 500, fwdeta0, 5.2};
    mj["HLT_PFJetFwd400"] = range{500, 600, fwdeta0, 5.2};
    mj["HLT_PFJetFwd450"] = range{500, 600, fwdeta0, 5.2};
    mj["HLT_PFJetFwd500"] = range{600,3000, fwdeta0, 5.2};

    mi["HLT_ZeroBias"] = range{10,  49,  0, 5.2};
    mi["HLT_MC"]       = range{15,6500,  0, 5.2};
    /*
    mi["HLT_PFJet40"]  = range{49,  84,  0, 5.2};
    mi["HLT_PFJet60"]  = range{84,  114, 0, 5.2};
    mi["HLT_PFJet80"]  = range{114, 196, 0, 5.2};
    mi["HLT_PFJet140"] = range{196, 272, 0, 5.2};
    mi["HLT_PFJet200"] = range{272, 330, 0, 5.2};
    mi["HLT_PFJet260"] = range{330, 395, 0, 5.2};
    mi["HLT_PFJet320"] = range{395, 468, 0, 5.2};
    mi["HLT_PFJet400"] = range{468, 548, 0, 5.2};
    mi["HLT_PFJet450"] = range{548, 686, 0, 5.2};
    mi["HLT_PFJet500"] = range{686,6500, 0, 5.2};
    */
    double fwdetatrk(3.0);
    mi["HLT_PFJet40"]  = range{49,  84,  0, fwdetatrk};
    mi["HLT_PFJet60"]  = range{84,  114, 0, fwdetatrk};
    mi["HLT_PFJet80"]  = range{114, 196, 0, fwdetatrk};
    mi["HLT_PFJet140"] = range{196, 272, 0, fwdetatrk};
    mi["HLT_PFJet200"] = range{272, 330, 0, fwdetatrk};
    mi["HLT_PFJet260"] = range{330, 395, 0, fwdetatrk};
    mi["HLT_PFJet320"] = range{395, 468, 0, fwdetatrk};
    mi["HLT_PFJet400"] = range{468, 548, 0, fwdetatrk};
    mi["HLT_PFJet450"] = range{548, 686, 0, fwdetatrk};
    mi["HLT_PFJet500"] = range{686,6500, 0, fwdetatrk};

    //JEC4Prompt
    
    mi["HLT_PFJetFwd40"]  = range{49,  84,  fwdetatrk, 5.2};
    mi["HLT_PFJetFwd60"]  = range{84,  114, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd80"]  = range{114, 196, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd140"] = range{196, 272, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd200"] = range{272, 330, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd260"] = range{330, 395, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd320"] = range{395, 468, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd400"] = range{468, 548, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd450"] = range{548, 686, fwdetatrk, 5.2};
    mi["HLT_PFJetFwd500"] = range{686,6500, fwdetatrk, 5.2};
    

    /*
    mi["HLT_PFJet40"]  = range{49,  84,  0, fwdeta0};
    mi["HLT_PFJet60"]  = range{84,  114, 0, fwdeta};
    mi["HLT_PFJet80"]  = range{114, 196, 0, fwdeta};
    mi["HLT_PFJet140"] = range{196, 272, 0, fwdeta};
    mi["HLT_PFJet200"] = range{272, 330, 0, fwdeta0};
    mi["HLT_PFJet260"] = range{330, 395, 0, fwdeta0};
    mi["HLT_PFJet320"] = range{395, 468, 0, fwdeta0};
    mi["HLT_PFJet400"] = range{468, 548, 0, fwdeta0};
    mi["HLT_PFJet450"] = range{548, 686, 0, fwdeta0};
    mi["HLT_PFJet500"] = range{686,6500, 0, fwdeta0};
    //mi["HLT_PFJet550"] = range{700,3000, 0, fwdeta0};
    
    mi["HLT_PFJetFwd40"]  = range{49,  84,  fwdeta0, 5.2};
    mi["HLT_PFJetFwd60"]  = range{84,  114, fwdeta, 5.2};
    mi["HLT_PFJetFwd80"]  = range{114, 196, fwdeta, 5.2};
    mi["HLT_PFJetFwd140"] = range{196, 272, fwdeta, 5.2};
    mi["HLT_PFJetFwd200"] = range{272, 330, fwdeta0, 5.2};
    mi["HLT_PFJetFwd260"] = range{330, 395, fwdeta0, 5.2};
    mi["HLT_PFJetFwd320"] = range{395, 468, fwdeta0, 5.2};
    mi["HLT_PFJetFwd400"] = range{468, 548, fwdeta0, 5.2};
    mi["HLT_PFJetFwd450"] = range{548, 686, fwdeta0, 5.2};
    mi["HLT_PFJetFwd500"] = range{686,6500, fwdeta0, 5.2};
    */
  }

  if (folder=="MCtruth" && (hist=="ptreco/ptgen" || hist=="response_ptgen" || hist=="response_etagen" ||
			    hist=="h2pteta" || hist=="h2pteta_gen" || hist=="h2pteta_rec" ||
			    hist=="p2jes" || hist=="p2jsf" || hist=="p2r" ||
                            hist=="p2r_NoMatch" || hist=="p2r_raw" || hist=="p2effz" ||
                            hist=="p2eff" || hist=="p2pur"))
    return true;

  bool tcHist = (folder=="Dijet2" && (hist=="h2ptetatc" || hist=="p2restc" ||
				      hist=="p2m0tc" || hist=="p2m2tc" ||
				      hist=="p2mntc" || hist=="p2mutc"));
  bool pfHist = (folder=="Dijet2" && (hist=="h2ptetapf" || hist=="p2respf" ||
				      hist=="p2m0pf" || hist=="p2m2pf" ||
				      hist=="p2mnpf" || hist=="p2mupf"));
  bool h3MPF = (folder=="Dijet2" && (hist=="h3m0" || hist=="h3m2"));

    if (folder=="Incjet" && (hist=="hpteta20" || hist=="hpteta30" ||
			   hist=="hpteta40" || hist=="hpteta50"))
    return true;

    if (folder=="Dijet" && (hist=="hetatag" || hist=="hetaprobe")){
      return true;
    }

    if (folder=="Multijet" && (hist=="hetalead" || hist=="hetarecoil")){
      return true;
    }

  bool h1JetRuns = (folder=="JetsperRuns" && (hist=="h1jetrate"));

  bool h3asymm = (folder=="Jetveto" && (hist=="Asymm"||hist=="h3asymm"));
  
  if (folder=="Jetveto" && (hist=="p2chf" || hist=="p2nhf" || hist=="p2nef" ||
			    hist=="p2asymm" || hist=="h2phieta" ||
			    hist=="h2phieta_ave" || hist=="p2asymm_noveto" ||
			    hist=="p2asymm_pttag" || hist=="p2asymm_noveto_pttag" ||
			    hist=="p2chf_pttag" || hist=="p2nhf_pttag" || hist=="p2nef_pttag" ||
			    hist=="h2phieta_pttag" || hist=="h2pteta_all_pttag" || hist=="h2pteta_sel_pttag" ||
			    hist=="p2chf_noveto_pttag" || hist=="p2nef_noveto_pttag" || hist=="p2nhf_noveto_pttag"))
    return true;
  if (folder=="Jetveto") {
    if (h3asymm) {
      if (mi.find(trg)!=mi.end() &&
          pt >= mi[trg].ptmin && pt < mi[trg].ptmax &&
          fabs(eta) >= mi[trg].absetamin && fabs(eta) < mi[trg].absetamax)
        return true;
    }
    else {
      if (mi.find(trg)!=mi.end() &&
          pt >= mi[trg].ptmin && pt < mi[trg].ptmax &&
          fabs(eta) >= mi[trg].absetamin && fabs(eta) < mi[trg].absetamax)
        return true;
    }
  }

  /*
  if (folder=="Incjet" &&
      mi.find(trg)!=mi.end() &&
      pt >= mi[trg].ptmin && pt < mi[trg].ptmax &&
      fabs(eta) >= mi[trg].absetamin && fabs(eta) < mi[trg].absetamax)
    return true;
  */
  if (folder=="Incjet") {

    string trigger("none");
    bool covered = false;
    for (auto& box : incjetBoxes) {
      // Check if (pt, eta) is in the region
      if (inRange(pt, box.ptMin, box.ptMax) &&
	  inRange(fabs(eta), box.absyMin, box.absyMax)) {

	// Copy bin content for this trigger
	trigger = box.triggerName;

	// Sanity check for overlapping boxes
	if (covered) {
	  std::cerr << "Warning: Histogram " << hist
		    << " trigger " << trigger
		    << " bin (pt=" << pt << ", eta=" << eta << ")"
		    << " already covered by another trigger!\n";
	}

	// Mark as covered and break if you expect only one coverage
	covered = true;
	//break;
      }
    }

    // Sanity check for missing phase space corners
    /*
    if (!covered) {
      std::cerr << "Warning: Histogram " << hist
		<< " bin (pt=" << pt
		<< ", eta=" << eta << ") not covered by any trigger!\n";
    }
    */

    if (trigger==trg) return true;
  } // Incjet
  if (folder=="Dijet" &&
      md.find(trg)!=md.end() &&
      pt >= md[trg].ptmin && pt < md[trg].ptmax &&
      fabs(eta) >= md[trg].absetamin && fabs(eta) < md[trg].absetamax)
    return true;

  if (folder=="Pileup" &&
      md.find(trg)!=md.end() &&
      pt >= md[trg].ptmin && pt < md[trg].ptmax &&
      fabs(eta) >= md[trg].absetamin && fabs(eta) < md[trg].absetamax)
    return true;

  if (folder=="MCtruth")// && 
      //md2.find(trg)!=md2.end() &&
      //pt >= md2[trg].ptmin && pt < md2[trg].ptmax &&
      //fabs(eta) >= md2[trg].absetamin && fabs(eta) < md2[trg].absetamax)
    return true; 
      
  if (folder=="Dijet2") {
    
    if (tcHist) { // pT,tag binning
      if (md2tc.find(trg)!=md2tc.end() &&
	  pt >= md2tc[trg].ptmin && pt < md2tc[trg].ptmax &&
	  fabs(eta) >= md2tc[trg].absetamin && fabs(eta) < md2tc[trg].absetamax)
	return true;
    }
    else if (pfHist) { // pT,probe binning
      if (md2pf.find(trg)!=md2pf.end() &&
	  pt >= md2pf[trg].ptmin && pt < md2pf[trg].ptmax &&
	  fabs(eta) >= md2pf[trg].absetamin && fabs(eta) < md2pf[trg].absetamax)
	return true;
    }
    else if (h3MPF) { // TH3D MPF binning
      if (md2pf.find(trg)!=md2pf.end() &&
          pt >= md2pf[trg].ptmin && pt < md2pf[trg].ptmax &&
          fabs(eta) >= md2pf[trg].absetamin && fabs(eta) < md2pf[trg].absetamax)
        return true;
    }
    else { // pT,AVP or pT,ave binning
      if (md2.find(trg)!=md2.end() &&
	  pt >= md2[trg].ptmin && pt < md2[trg].ptmax &&
	  fabs(eta) >= md2[trg].absetamin && fabs(eta) < md2[trg].absetamax)
	return true;
    }
  } // Dijet2
  // 20% higher thresholds for multijet recoil binning
  if (folder=="Multijet") {
    double k(1);
    if (hist=="hptr_all" || hist=="hptr_sel" || hist=="presr" ||
	hist=="pcrecoilr" || hist=="pm0r" || hist=="pm2r" ||
	hist=="pmnr" || hist=="pmur") k = 1.15;
    if (mi.find(trg)!=mi.end() &&
	pt >= k*mi[trg].ptmin && pt < k*mi[trg].ptmax &&
	fabs(eta) >= mi[trg].absetamin && fabs(eta) < mi[trg].absetamax)
    return true;
  }
  if (folder=="JetsperRuns") {
    if (h1JetRuns) {
      if (md2pf.find(trg)!=md2pf.end() &&
          pt >= md2pf[trg].ptmin && pt < md2pf[trg].ptmax &&
          fabs(eta) >= md2pf[trg].absetamin && fabs(eta) < md2pf[trg].absetamax)
        return true;
    }
    else {
      if (mi.find(trg)!=mi.end() &&
          pt >= mi[trg].ptmin && pt < mi[trg].ptmax &&
          fabs(eta) >= mi[trg].absetamin && fabs(eta) < mi[trg].absetamax)
        return true;
    }
  }

  // else
  return false;
   //if (trg=="HLT_PFJet450" && pt>500 && pt<6500 && fabs(eta)<3.0) return true;
}
