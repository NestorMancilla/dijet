#define DijetHistosFill_cxx
//#define NANOV12
//#define NANOV09
//#ifdef NANOVO9
//      #include "../interface/DijetHistosFillV09.h" // Specific for NanoV09
//#else
//     #include "../interface/DijetHistosFill.h" // Specific for NanoV12 (assumed default)
//#endif
//#include "../interface/DijetHistosFillNanoV9.h"
//#include "../interface/DijetHistosFill.h"
#include "../interface/DijetHistosFill_2024Prompt.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "TLorentzVector.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TStopwatch.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <string>
// #include <utility>
#include <random>
#include <sstream>
#include <unordered_map>
#include <array>
#include <string_view>
#include <algorithm>

// Recalculate JECs
bool redoJEC = true;

// MC triggers (slow) or not (faster)
bool doMCtrigOnly = true;

// JER smearing (JER SF)
bool smearJets = false;
bool useJERSFvsPt = false; // new file format
int smearNMax = 3;
std::uint32_t _seed;
std::mt19937 _mersennetwister;

// Do PU reweighting and studies
bool reweightPU = true;
bool doPU_per_trigger = false;
bool do_PUProfiles = true;

// Activate modules
bool doJetveto = true; // eta-phi maps
bool doMCtruth = false;
bool doIncjet = true;   // inclusive jets
bool doDijet = true;    // dijet selection
bool doGluonJets = false; //  MPF/DB calculations for dijet using Jet_btagPNetQvG per workingpoint
bool doDijet2 = true;   // dijet selection (DESY style)
bool doMultijet = true; // multijet selection
bool doJetsperRuns = false; // Jets rate per runs normalized by the luminosity

// Core additions
bool doPFComposition = true; // jetveto / incjet / dijet / multijet
bool doDijetJER = true;


// Additional variants and controls
bool doJetvetoVariants = true;
bool doMultijetControl = true;
bool doMultijet2Drecoil = true;
bool doDijet2NM = false; // true;
bool doJetID = false;    // add JetID class

bool debug = false;      // general debug
bool debugevent = false; // per-event debug

// Maximum asymmetry of 2/3 corresponds to x2 ratio of tag and probe
// Permit ~0.7 extra scaling to allow for HF L3Res
const double maxa = 10; // no cut with 10

const double WP_t = 0.16;
const double WP_m = 0.33;
const double WP_l = 0.55;

// List of MC IOVs
std::set<std::string> mcIOV = {"Summer22",
                               "Summer22Flat", "Summer22EE", "Summer22EEFlat",
			       "Summer22EEMG_full", "Summer22MG_full",
                               "Summer23", "Summer23MCFlat", "Summer23MG",
                               "Summer23BPixFlat", "Summer23BPIXMG", "2023BCv123",
                               "2023BCv123_ZB", "2023Cv4", "2023Cv4_ZB", "2023D", "2023D_ZB",
                               "Summer22MG", "Summer22EEMG", "Summer22MG1", "Summer22MG2",
                               "Summer22EEMG1", "Summer22EEMG2", "Summer22EEMG3", "Summer22EEMG4",
                               "TestSummer23MGBPix",
			       "Summer23MC_Cas"
                               "Winter24MCFlat", "Winter24MG"};

// UTILITIES
double DELTAPHI(double phi1, double phi2)
{
  double dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi() ? dphi : TMath::TwoPi() - dphi);
}
double DELTAR(double phi1, double phi2, double eta1, double eta2)
{
  return sqrt(pow(DELTAPHI(phi1, phi2), 2) + pow(eta1 - eta2, 2));
}



constexpr const char lumibyls2022C[] = "luminosityscripts/csvfiles/lumibyls2022C.csv";
constexpr const char lumibyls2022D[] = "luminosityscripts/csvfiles/lumibyls2022D.csv";
constexpr const char lumibyls2022E[] = "luminosityscripts/csvfiles/lumibyls2022E.csv";
constexpr const char lumibyls2022F[] = "luminosityscripts/csvfiles/lumibyls2022F.csv";
constexpr const char lumibyls2022G[] = "luminosityscripts/csvfiles/lumibyls2022G.csv";
constexpr const char lumibyls2023B[] = "luminosityscripts/csvfiles/lumibyls2023AB.csv";
constexpr const char lumibyls2023C4[] = "luminosityscripts/csvfiles/lumibyls2023C4.csv";
constexpr const char lumibyls2023C123[] = "luminosityscripts/csvfiles/lumibyls2023C123.csv";
constexpr const char lumibyls2023ABC[] = "luminosityscripts/csvfiles/lumibyls2023ABC.csv";
constexpr const char lumibyls2023D[] = "luminosityscripts/csvfiles/lumibyls2023D.csv";
//constexpr const char lumibyls2023D[] = "luminosityscripts/csvfiles/lumibyrun2023_366442_370790_Golden.csv";
//constexpr const char lumibyls2024BC[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_379866_Golden.csv";
//constexpr const char lumibyls2024BC[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_380115_Golden.csv";
//constexpr const char lumibyls2024BC[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_380649_DCSOnly.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_380470_Golden.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_380649_DCSOnly.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_381199_DCSOnly.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_380649_Golden.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumi_GoldenRuns_378985to380945_DCSRuns_380946to381516.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumi_GoldenRuns_378985to381152_DCSRuns_381153to381594.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumi_GoldenRuns_378985to381417_DCSRuns_381418to381594.csv";
//constexpr const char lumibyls2024BCDEF[] = "luminosityscripts/csvfiles/lumi_GoldenRuns_378981to382329_DCSRuns_382330to382686.csv";
//constexpr const char lumibyls2024BCDEF[] = "luminosityscripts/csvfiles/lumi_GoldenRuns_378985to383163_DCSRuns_383164to383467.csv";
constexpr const char lumibyls2024BCDEFG[] = "luminosityscripts/csvfiles/lumi_GoldenRuns_378981to383724_DCSRuns_383725to384446.csv";
constexpr const char lumibyls2024ECALB[] = "luminosityscripts/csvfiles/lumibyrun2024_eraB_Golden.csv";
constexpr const char lumibyls2024eraB[] = "luminosityscripts/csvfiles/lumibyrun2024_eraB_Golden.csv";
//constexpr const char lumibyls2024BCDE[] = "luminosityscripts/csvfiles/lumibyrun2024_378981_381478_DCSOnly.csv";

constexpr std::array<std::pair<const char*, const char*>, 42> lumifiles = {{
    {"2022C", lumibyls2022C},
    {"2022C_ZB", lumibyls2022C},
    {"2022D", lumibyls2022D},
    {"2022D_ZB", lumibyls2022D},
    {"2022E", lumibyls2022E},
    {"2022E_ZB", lumibyls2022E},
    {"2022F", lumibyls2022F},
    {"2022F_ZB", lumibyls2022F},
    {"2022G", lumibyls2022G},
    {"2022G_ZB", lumibyls2022G},
    {"2023B", lumibyls2023B},
    {"2023B_ZB", lumibyls2023B},
    {"2023Cv4", lumibyls2023C4},
    {"2023Cv4_ZB", lumibyls2023C4},
    {"2023Cv123", lumibyls2023C123},
    {"2023BCv123", lumibyls2023ABC},
    {"2023Cv123_ZB", lumibyls2023C123},
    {"2023D", lumibyls2023D},
    {"2023D_ZB", lumibyls2023D},
    //{"2024B", lumibyls2024eraB}, //Luminosity per run for prompt 2024BC 
    //{"2024B_ZB", lumibyls2024eraB}, //Luminosity per run for prompt 2024BC 
    {"2024B", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024BC 
    {"2024B_ZB", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024BC 
    {"2024C", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024BC
    {"2024C_ZB", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024BC
    {"2024D", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024D
    {"2024D_ZB", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024D
    {"2024Ev1", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024E
    {"2024Ev1_ZB", lumibyls2024BCDEFG}, //Luminosity per run for prompt 2024E
    {"2024BR", lumibyls2024BCDEFG},
    {"2024CR", lumibyls2024BCDEFG},
    {"2024Ev2", lumibyls2024BCDEFG},
    {"2024Ev2_ZB", lumibyls2024BCDEFG},
    {"2024CS", lumibyls2024BCDEFG},
    {"2024CT", lumibyls2024BCDEFG},
    {"2024F", lumibyls2024BCDEFG},
    {"2024F_1", lumibyls2024BCDEFG},
    {"2024F_2", lumibyls2024BCDEFG},
    {"2024F_3", lumibyls2024BCDEFG},
    {"2024F_4", lumibyls2024BCDEFG},
    {"2024F_ZB", lumibyls2024BCDEFG},
    {"2024G", lumibyls2024BCDEFG},
    {"2024G_ZB", lumibyls2024BCDEFG},
}}; // NOT CORRECT FOR 2023BCv123!!!! TEMP. FIX WHILE LUMI IS STILL NOT IN USE

constexpr const char *getLumifile(const char* dataset, std::size_t index = 0)
{
    if (index >= lumifiles.size())
        return nullptr; // Dataset not found

    if (std::strcmp(lumifiles[index].first, dataset) == 0)
        return lumifiles[index].second;
    
    return getLumifile(dataset, index + 1);
}


// Hardcoded pT, eta thresholds for each trigger
// used in e.g. jetvetoHistos
struct range
{
  double ptmin;
  double ptmax;
  double absetamin;
  double absetamax;
};
std::map<std::string, struct range> mt;
// Hardcoded pT, eta thresholds for each trigger
// used in e.g. jetrate
std::map<std::string, struct range> mi;
std::map<std::string, struct range> md;
std::map<std::string, struct range> md2;
std::map<std::string, struct range> md2tc;
std::map<std::string, struct range> md2pf;
std::map<std::string, struct range> mj;

// CLASS DEFINITIONS
class mctruthHistos
{
public:
  TH1D *ptreco_ptgen; //, *h1res_bar;
  TH2D *h2pteta, *h2pteta_gen, *h2pteta_rec, *h2res_ptgen, *h2res_etagen, *h2_btagUpar;//, *h2res_bar;
  TH3D *h3res, *h3res_Match, *h3res_raw;
  TProfile2D *p2jes, *p2jsf, *p2r, *p2r_NoMatch, *p2r_raw, *p2effz, *p2eff, *p2pur;
};

class jetvetoHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  // Jet counts
  TH2D *h2pteta_all, *h2pteta_sel, *h2phieta;
  TH2D *h2ptaeta_all, *h2ptaeta_sel, *h2phieta_ave;
  TH2D *h2pteta_all_pttag, *h2pteta_sel_pttag, *h2phieta_pttag;

  // Asymm
  TH3D *h3asymm;

  // Balancing
  TProfile2D *p2asymm, *p2asymm_noveto, *p2asymm_pttag, *p2asymm_noveto_pttag;
  //Tprofile2D *p2asymm_noveto;

  // (Optional) composition plots
  TProfile2D *p2chf, *p2nhf, *p2nef;
  TProfile2D *p2chf_pttag, *p2nhf_pttag, *p2nef_pttag, *p2chf_noveto_pttag, *p2nhf_noveto_pttag, *p2nef_noveto_pttag;
};

class incjetHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  static const int ny = 10;
  TH2D *h2pteta_all;
  TH2D *h2pteta_sel;
  TH2D *h2pteta, *h2pteta_lumi;
  TH1D *hpt13, *hpteta20, *hpteta30, *hpteta40, *hpteta50;
  TH1D *vpt[ny];

  // Control plots for pileup
  TH2D *h2jtvht, *h2jtoht;

  // (Optional) composition plots
  TProfile2D *p2pt, *p2rho, *p2rho_PURW, *p2chf, *p2nef, *p2nhf, *p2cef, *p2muf;
  TProfile *ppt13, *prho13, *pchf13, *pnef13, *pnhf13, *pcef13, *pmuf13;
  // Related to the tails issue. May 17, 2024. Nestor
  TH2D *h2chf13;
};

class dijetHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  TH2D *h2pteta_aball, *h2pteta_absel;
  TH2D *h2pteta_adall, *h2pteta_adsel;
  TH2D *h2pteta_tcall, *h2pteta_tcsel;
  TH2D *h2pteta_pfall, *h2pteta_pfsel;
  TProfile2D *p2resab, *p2resad, *p2restc, *p2respf; // JEC L2L3Res for undoing
  TProfile2D *p2m0, *p2m0x, *p2m2, *p2m2x;           // JER MPFX, DBX methods
  TProfile2D *p2m0ab, *p2m2ab, *p2mnab, *p2muab;     // pT,avp (bisector)
  TProfile2D *p2m0ad, *p2m2ad, *p2mnad, *p2muad;     // pT,ave (dijet axis)
  TProfile2D *p2m0tc, *p2m2tc, *p2mntc, *p2mutc;     // pT,tag (central)
  TProfile2D *p2m0pf, *p2m2pf, *p2mnpf, *p2mupf;     // pt,probe (forward)

  // (Optional) composition plots
  TProfile2D *p2pt, *p2rho, *p2chf, *p2nef, *p2nhf, *p2cef, *p2muf;      // probe,avp
  TProfile *ppt13, *prho13, *pchf13, *pnef13, *pnhf13, *pcef13, *pmuf13; // tag
};

class gluonHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  TProfile2D *p2m0_t, *p2m0x_t, *p2m2_t, *p2m2x_t;           // JER MPFX, DBX methods
  TProfile2D *p2m0ab_t, *p2m2ab_t, *p2mnab_t, *p2muab_t;     // pT,avp (bisector)
  TProfile2D *p2m0ad_t, *p2m2ad_t, *p2mnad_t, *p2muad_t;     // pT,ave (dijet axis)
  TProfile2D *p2m0tc_t, *p2m2tc_t, *p2mntc_t, *p2mutc_t;     // pT,tag (central)
  TProfile2D *p2m0pf_t, *p2m2pf_t, *p2mnpf_t, *p2mupf_t;     // pt,probe (forward)

  TProfile2D *p2m0_m, *p2m0x_m, *p2m2_m, *p2m2x_m;           // JER MPFX, DBX methods
  TProfile2D *p2m0ab_m, *p2m2ab_m, *p2mnab_m, *p2muab_m;     // pT,avp (bisector)
  TProfile2D *p2m0ad_m, *p2m2ad_m, *p2mnad_m, *p2muad_m;     // pT,ave (dijet axis)
  TProfile2D *p2m0tc_m, *p2m2tc_m, *p2mntc_m, *p2mutc_m;     // pT,tag (central)
  TProfile2D *p2m0pf_m, *p2m2pf_m, *p2mnpf_m, *p2mupf_m; 

  TProfile2D *p2m0_l, *p2m0x_l, *p2m2_l, *p2m2x_l;           // JER MPFX, DBX methods
  TProfile2D *p2m0ab_l, *p2m2ab_l, *p2mnab_l, *p2muab_l;     // pT,avp (bisector)
  TProfile2D *p2m0ad_l, *p2m2ad_l, *p2mnad_l, *p2muad_l;     // pT,ave (dijet axis)
  TProfile2D *p2m0tc_l, *p2m2tc_l, *p2mntc_l, *p2mutc_l;     // pT,tag (central)
  TProfile2D *p2m0pf_l, *p2m2pf_l, *p2mnpf_l, *p2mupf_l; 

};

class dijetHistos2
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  TH2D *h2pteta;
  TProfile2D *p2res, *p2m0, *p2m2, *p2mn, *p2mu;
  TProfile2D *p2m0x, *p2m2x;
  TH3D *h3m0, *h3m2;

  // Extra for FSR studies
  TProfile2D *p2mnu, *p2mnx, *p2mux, *p2mnux;
  TH2D *h2ptetatc, *h2ptetapf;
  TProfile2D *p2restc, *p2m0tc, *p2m2tc, *p2mntc, *p2mutc; // pT,tag (central)
  TProfile2D *p2respf, *p2m0pf, *p2m2pf, *p2mnpf, *p2mupf; // pT,probe (forward)

  // Smearing controls
  TProfile2D *p2jsf, *p2jsftc, *p2jsfpf;
};

class multijetHistos
{
public:
  // Basic information about the trigger
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;

  TProfile *ptleada, *ptleadm, *ptleadl, *ptleadr;
  TProfile *pcrecoila, *pcrecoilm, *pcrecoill, *pcrecoilr;
  TH1D *hpta_all, *hptm_all, *hptl_all, *hptr_all;
  TH1D *hpta_sel, *hptm_sel, *hptl_sel, *hptr_sel;
  TProfile *presa, *presm, *presl, *presr;
  TProfile *pm0a, *pm2a, *pmna, *pmua; // *pmoa; // pT,avp3
  TProfile *pm0m, *pm2m, *pmnm, *pmum; // *pmom; // pT,ave
  TProfile *pm0l, *pm2l, *pmnl, *pmul; //*pmol; // pT,tag
  TProfile *pm0r, *pm2r, *pmnr, *pmur; // *pmor; // pT,probe

  // (Optional) 2D recoils
  TH2D *h2recoila, *h2recoilm, *h2recoill, *h2recoilr;

  // (Optional) composition plots
  TProfile *ppt13, *prho13, *pchf13, *pnef13, *pnhf13, *pcef13, *pmuf13; // lead pT,avp
  TProfile *ppt25, *prho25, *pchf25, *pnef25, *pnhf25, *pcef25, *pmuf25; // recoil,pT,avp

  // (Optional) Controls
  TH2D *h2m0a;
  TH2D *h2m2a;
  TH1D *hcosdphi;
};

class lumiHistos
{
public:
  string trg;
  int trgpt;
  float lumsum, lumsum2, lum, lum2; // lum is recorded, lum2 delivered
  TH1D *htrglumi, *htrgpu, *hnpv, *hnpvgood;
  //TH1F *hLumiPerRun;
};

class jetsperRuns
{
public:
  string trg;
  int trgpt;
  double ptmin, ptmax, absetamin, absetamax;
  // Jet rate per trigger
  TH1D *h1jetxsec, *h1jetrate, *h1pt13, *h1pt13_w;
  TH2D *h2jetpteta;
  TProfile *pMPF_500, *pMPF_500b, *pMPF_600, *pMPF_600b, *pMPF_800, *pMPF_800b, *pMPF_1000, *pMPF_1000b, *pMPF_1200, *pMPF_1200b;
  TProfile *pDB_500, *pDB_500b, *pDB_600, *pDB_600b, *pDB_800, *pDB_800b, *pDB_1000, *pDB_1000b, *pDB_1200, *pDB_1200b;
  TProfile2D *p2MPF, *p2MPF_bar;
};

// Helper function to retrieve FactorizedJetCorrector
FactorizedJetCorrector *getFJC(string l1 = "", string l2 = "", string res = "",
                               string path = "")
{

  // Set default jet algo
  if (l1 != "" && !(TString(l1.c_str()).Contains("_AK")))
    l1 += "_AK4PFPuppi";
  if (l2 != "" && !(TString(l2.c_str()).Contains("_AK")))
    l2 += "_AK4PFPuppi";
  if (res != "" && !(TString(res.c_str()).Contains("_AK")))
    res += "_AK4PFPuppi";

  // Set default path
  if (path == "")
    path = "CondFormats/JetMETObjects/data";
  const char *cd = path.c_str();
  const char *cl1 = l1.c_str();
  const char *cl2 = l2.c_str();
  const char *cres = res.c_str();
  string s("");

  vector<JetCorrectorParameters> v;
  if (l1 != "")
  {
    s = Form("%s/%s.txt", cd, cl1);
    cout << s << endl
         << flush;
    JetCorrectorParameters *pl1 = new JetCorrectorParameters(s);
    v.push_back(*pl1);
  }
  if (l2 != "")
  {
    s = Form("%s/%s.txt", cd, cl2);
    cout << s << endl
         << flush;
    JetCorrectorParameters *pl2 = new JetCorrectorParameters(s);
    v.push_back(*pl2);
  }
  if (res != "")
  {
    s = Form("%s/%s.txt", cd, cres);
    cout << s << endl
         << flush;
    JetCorrectorParameters *pres = new JetCorrectorParameters(s);
    v.push_back(*pres);
  }
  FactorizedJetCorrector *jec = new FactorizedJetCorrector(v);

  return jec;
} // getFJC
/////////////////////////
/////////////////////////
//// PU reweighting
//// Nestor. 10, 2024.
/////////////////////////
/////////////////////////

/////////////////////////
//Get the PU from the root files
/////////////////////////
void DijetHistosFill::get_PU_hist(const std::string& PUdataset) {
    // Open the ROOT file
    //std::string file_path = "luminosityscripts/PUWeights/PU_weights_2024_trgs.root";
    std::string file_path = "luminosityscripts/PUWeights/PU_weights_2024_trgs_v2.root";
    TFile* root_file = TFile::Open(file_path.c_str(), "READ");

    // Check if the file opened successfully
    if (!root_file || root_file->IsZombie()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return;
    }

    // Get the directory corresponding to the dataset
    //TDirectory* dir = (TDirectory*)root_file->Get(PUdataset);
    TDirectory* dir = (TDirectory*)root_file->Get(PUdataset.c_str());
    if (!dir) {
        std::cerr << "Directory " << PUdataset << " not found in file." << std::endl;
        root_file->Close();
        return;
    } else{
        std::cerr << "Directory " << PUdataset << " found in file." << std::endl;
    }


    // List of triggers
    std::vector<std::string> vtrg = {
	"HLT_ZeroBias",
        "HLT_PFJet40", "HLT_PFJet60", "HLT_PFJet80", "HLT_PFJet140", "HLT_PFJet200",
        "HLT_PFJet260", "HLT_PFJet320", "HLT_PFJet400", "HLT_PFJet450", "HLT_PFJet500",
        "HLT_PFJetFwd40", "HLT_PFJetFwd60", "HLT_PFJetFwd80", "HLT_PFJetFwd140", "HLT_PFJetFwd200",
        "HLT_PFJetFwd260", "HLT_PFJetFwd320", "HLT_PFJetFwd400", "HLT_PFJetFwd450", "HLT_PFJetFwd500",
        "HLT_DiPFJetAve40", "HLT_DiPFJetAve60", "HLT_DiPFJetAve80", "HLT_DiPFJetAve140", "HLT_DiPFJetAve200",
        "HLT_DiPFJetAve260", "HLT_DiPFJetAve320", "HLT_DiPFJetAve400", "HLT_DiPFJetAve500",
        "HLT_DiPFJetAve60_HFJEC", "HLT_DiPFJetAve80_HFJEC", "HLT_DiPFJetAve100_HFJEC",
        "HLT_DiPFJetAve160_HFJEC", "HLT_DiPFJetAve220_HFJEC", "HLT_DiPFJetAve300_HFJEC"
    };

    // Clear the existing histograms (if necessary)
    pu_hist_map.clear();

    // Loop over each trigger and retrieve the corresponding histogram
    for (const auto& trg : vtrg) {
        std::string hist_name = "PU_weights_" + trg;
        TH1D* hist = (TH1D*)dir->Get(hist_name.c_str());
	//std::cerr << "hist name string " << hist_name.c_str() << std::endl;

        if (hist && !hist->IsZombie() && hist->InheritsFrom(TH1::Class())) {
            //std::cout << "Found histogram " << hist_name << " in directory." << std::endl;
            TH1D* cloned_hist = (TH1D*)hist->Clone();
            if (cloned_hist) {
		cloned_hist->SetDirectory(0);
                pu_hist_map[trg] = cloned_hist;
                // Verification step: Print histogram name and number of bins
                //std::cout << "Histogram " << hist_name << " added to pu_hist_map with " 
                  //        << cloned_hist->GetNbinsX() << " bins." << std::endl;
            } else {
                std::cerr << "Failed to clone histogram " << hist_name << std::endl;
            }
        } else {
            std::cerr << "Histogram " << hist_name << " not found in directory " << PUdataset << std::endl;
        }
    }

    // Close the ROOT file
    root_file->Close();
}


////
// Function to verify the mapping is correct
////
void DijetHistosFill::print_PU_hist_map() const {
    for (const auto& pair : pu_hist_map) {
        if (pair.second && pair.second->InheritsFrom(TH1::Class())) {
            std::cout << "Trigger: " << pair.first
                      << ", Histogram: " << pair.second->GetName()
                      << std::endl;
        } else {
            std::cerr << "Trigger: " << pair.first << " has a null or invalid histogram." << std::endl;
        }
    }
}

/////////////////////////
//End PU from the root files
/////////////////////////
//// Function to get the PU_weight
/////////////////////////
void DijetHistosFill::get_weight(float pt, float eta, string analysis) {

    eta = std::abs(eta);
    
    std::map<std::string, struct range>* selected_map = nullptr;
    if (analysis == "doDijet") {
        selected_map = &md;
    } else if (analysis == "doDijet2") {
        selected_map = &md2;
    } else if (analysis == "doDijet2_pf") {
        selected_map = &md2pf;
    } else if (analysis == "doDijet2_tc") {
        selected_map = &md2tc;
    } else if (analysis == "doInc" || analysis == "doMultijets" || analysis == "doJetveto") {
        selected_map = &mi;
    } else {
        std::cerr << "Unknown pt_analysis type: " << analysis << std::endl;
	double PU_weight = 1;
        return;
    }

    std::string selected_trigger = "";
    for (const auto& entry : *selected_map) {
        const std::string& trigger_name = entry.first;
        const range& trigger_range = entry.second;

        if (pt >= trigger_range.ptmin && pt < trigger_range.ptmax &&
            eta >= trigger_range.absetamin && eta < trigger_range.absetamax) {
            selected_trigger = trigger_name;
	    //std::cerr << "selected_trigger:  " << selected_trigger << std::endl;
            break;
        }
    }

    //std::cerr << "selected_trigger after the function selection:  " << selected_trigger << std::endl;

    if (selected_trigger.empty()) {
        //std::cerr << "No matching trigger found for pt: " << pt << " and eta: " << eta << std::endl;
        PU_weight = 1;
        return;
    }

    //auto hist_it = pu_hist_map.find("PU_weights_" + selected_trigger);
    auto hist_it = pu_hist_map.find(selected_trigger);
    if (hist_it == pu_hist_map.end()) {
        std::cerr << "No histogram found for trigger: " << selected_trigger << std::endl;
        PU_weight = 1;
        return;
    } 


    TH1D* h1_PU_weight = hist_it->second;
    int ibin = h1_PU_weight->FindBin(Pileup_nTrueInt);
    //std::cerr << "h1_PU_weight->GetBinContent(ibin):  " << h1_PU_weight->GetBinContent(ibin) << std::endl;
    PU_weight = h1_PU_weight->GetBinContent(ibin);

}
/////////////////////////
//// End Function to get the PU_weight
/////////////////////////
/////////////////////////
bool DijetHistosFill::LoadLumi()
{
	// To read the luminosity based on the .csv file and take only events with non-zero luminosity
	vector<string> vtrg = {
		"HLT_PFJet40",
		"HLT_PFJet60",
		"HLT_PFJet80",
		"HLT_PFJet140",
		"HLT_PFJet200",
		"HLT_PFJet260",
		"HLT_PFJet320",
		"HLT_PFJet400",
		"HLT_PFJet450",
		"HLT_PFJet500",
		"HLT_PFJetFwd40", //
		"HLT_PFJetFwd60",
		"HLT_PFJetFwd80",
		"HLT_PFJetFwd140",
		"HLT_PFJetFwd200",
		"HLT_PFJetFwd260",
		"HLT_PFJetFwd320",
		"HLT_PFJetFwd400",
		"HLT_PFJetFwd450",
		"HLT_PFJetFwd500", //
		"HLT_DiPFJetAve40",
		"HLT_DiPFJetAve60",
		"HLT_DiPFJetAve80",
		"HLT_DiPFJetAve140",
		"HLT_DiPFJetAve200",
		"HLT_DiPFJetAve260",
		"HLT_DiPFJetAve320",
		"HLT_DiPFJetAve400",
		"HLT_DiPFJetAve500",
		"HLT_DiPFJetAve60_HFJEC",
		"HLT_DiPFJetAve80_HFJEC",
		"HLT_DiPFJetAve100_HFJEC",
		"HLT_DiPFJetAve160_HFJEC",
		"HLT_DiPFJetAve220_HFJEC",
		"HLT_DiPFJetAve300_HFJEC"};

	//string JSON_version = "378981_381199_DCSOnly"; // 2024 Prompt
	//string JSON_version = "366442_370790_Golden"; //2023 Golden
	//string JSON_version = "378981_380649_Golden";
	//string JSON_version = "eraB_Golden";
	//string JSON_version = "378981_381478_DCSOnly";
	//string JSON_version = "GoldenRuns_378985to380945_DCSRuns_380946to381516";
	//string JSON_version = "GoldenRuns_378985to381152_DCSRuns_381153to381594";
	//string JSON_version = "GoldenRuns_378981to382329_DCSRuns_382330to382686";
	//string JSON_version = "2022_Golden";
	string JSON_version = "GoldenRuns_378981to383724_DCSRuns_383725to384446";
	// List of filenames
	vector<string> filenames = {
		"luminosityscripts/csvfiles/lumi_HLT_PFJet40_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet60_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet80_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet140_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet200_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet260_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet320_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet400_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet450_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJet500_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd40_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd60_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd80_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd140_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd200_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd260_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd320_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd400_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd450_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_PFJetFwd500_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve40_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve60_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve80_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve140_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve200_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve260_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve320_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve400_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve500_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve60_HFJEC_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve80_HFJEC_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve100_HFJEC_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve160_HFJEC_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve220_HFJEC_"+JSON_version+".csv",
		"luminosityscripts/csvfiles/lumi_HLT_DiPFJetAve300_HFJEC_"+JSON_version+".csv"
	};

	for (size_t idx = 0; idx < vtrg.size(); ++idx) {
		const string& trigger = vtrg[idx];
		const string& filename = filenames[idx];

		ifstream file(filename);

		// Check if the file opened successfully
		if (!file.is_open()) {
			cerr << "Error opening file: " << filename << endl;
			continue;
		}

		string line;
		while (getline(file, line)) {
			// Skip comment lines
			if (line[0] == '#') continue;

			stringstream ss(line);
			string run_fill, time, ncms, hltpath, delivered, recorded;

			// Parse the line
			getline(ss, run_fill, ',');
			getline(ss, time, ',');
			getline(ss, ncms, ',');
			getline(ss, hltpath, ',');
			getline(ss, delivered, ',');
			getline(ss, recorded, ',');

			// Check if the base trigger name matches
			if (hltpath.find(trigger) != string::npos) {
				// Extract run number from run:fill
				int run = stoi(run_fill.substr(0, run_fill.find(':')));

				// Convert recorded luminosity to double
				double lum = stod(recorded);

				// Store the run and luminosity in the map
				mlumi[trigger][run] = lum; // * 1000. to have it in pb
			}
		}
		// Close the file
		file.close();
	}
	/*
	   for (const auto& trigger : mlumi) {
	   cout << "Trigger: " << trigger.first << endl;
	   for (const auto& run : trigger.second) {
	   cout << "  Run: " << run.first << ", Luminosity: " << run.second << endl;
	   }
	   }
	   */

	const char *lumifile = getLumifile(dataset.c_str());

	PrintInfo(string("Processing LoadLumi() with ") + lumifile + "...", true);

	// Check lumi against the list of good runs
	const int a_goodruns[] = {};
	const int ngoodruns = sizeof(a_goodruns) / sizeof(a_goodruns[0]);
	set<int> goodruns;
	if (ngoodruns > 0)
	{ // This is an old remnant
		for (int runidx = 0; runidx != ngoodruns; ++runidx)
			goodruns.insert(a_goodruns[runidx]);

		for (auto runit = goodruns.begin(); runit != goodruns.end(); ++runit)
			cout << *runit << ", ";
		cout << endl;
	}
	set<pair<int, int>> nolums;

	ifstream f(lumifile, ios::in);
	if (!f.is_open())
		return false;
	float secLS = 2.3310e+01;
	string s;
	int rn, fill, numls, ifoo;
	//int rn, fill, ls, ifoo;
	float del, rec, avgpu, energy;
	char sfoo[512];

	// Determine expected data tags based on the lumifile year
	std::string expectedTag;
	std::string expectedHeader;
	TString lumifile_str(lumifile);
	// Check the lumifile year and set the expected data tag and header accordingly
	if (lumifile_str.Contains("2022") || lumifile_str.Contains("2023")) {
		expectedTag = "#Data tag : 23v1 , Norm tag: None";
		expectedHeader = "#run:fill,ls,time,beamstatus,E(GeV),delivered(/ub),recorded(/ub),avgpu,source";
	} else {
		expectedTag = "#Data tag : 24v1 , Norm tag: None";
		expectedHeader = "#run:fill,time,nls,ncms,delivered(/fb),recorded(/fb)";
	}

	// Read and validate the first line
	bool getsuccess1 = static_cast<bool>(getline(f, s, '\n'));
	if (!getsuccess1) {
		std::cout << "Error reading the first line" << std::endl;
		return false;
	}

	PrintInfo("First line: " + s + " !", true);

	if (s != expectedTag) {
		std::cout << "First line does not match expected data tag" << std::endl;
		return false;
	}

	// Read and validate the second line
	bool getsuccess2 = static_cast<bool>(getline(f, s, '\n'));
	if (!getsuccess2) {
		std::cout << "Error reading the second line" << std::endl;
		return false;
	}

	PrintInfo("Second line: " + s + " !", true);

	if (s != expectedHeader) {
		std::cout << "Second line does not match expected header" << std::endl;
		return false;
	}

	/*
	   bool getsuccess1 = static_cast<bool>(getline(f, s, '\n'));
	   if (!getsuccess1)
	   return false;
	   PrintInfo(string("\nstring: ") + s + " !", true);

	   TString lumifile_str(lumifile);
	// HOX: the lumi file format has been changing. Change the conditions when needed.
	if (lumifile_str.Contains("2022") || lumifile_str.Contains("2023")) {
	if (s != "#Data tag : 23v1 , Norm tag: None")
	return false;
	}
	else {
	if (s != "#Data tag : 24v1 , Norm tag: None")
	return false;
	}
	//if (s != "#Data tag : 23v1 , Norm tag: None")
	//if (s != "#Data tag : 24v1 , Norm tag: None") {
	//return false;
	//}

	bool getsuccess2 = static_cast<bool>(getline(f, s, '\n'));
	if (!getsuccess2) {
	std::cout << "It is not passing the second line" << std::endl;
	return false;
	}
	PrintInfo(string("\nstring: ") + s + " !", true);

	//TString lumifile_str(lumifile);
	if (lumifile_str.Contains("2022") || lumifile_str.Contains("2023")){
	//if (s != "#run:fill,nls,time,beamstatus,E(GeV),delivered(/ub),recorded(/ub),avgpu,source") {
	if (s != "#run:fill,ls,time,beamstatus,E(GeV),delivered(/ub),recorded(/ub),avgpu,source") {
	std::cout << "2022 and 2023 marked as false" << std::endl;
	return false;
	}
	}
	else {
	if (s != "#run:fill,time,nls,ncms,delivered(/fb),recorded(/fb)") {
	std::cout << "The rest is marked as false" << std::endl;
	return false;
	}
	}
	*/
	//return false;

	int nls(0);
	double lumsum(0);
	double lumsum_good(0);
	double lumsum_json(0);
	double lum = 0.0;
	double lum2 = 0.0;
	bool skip(false);
	std::set<double> runNumbers;
	while (getline(f, s, '\n'))
	{
		//std::cout << "Processing line: " << s << std::endl;
		// Skip if not STABLE BEAMS or wrong number of arguments
		// STABLE BEAMS alts: ADJUST, BEAM DUMP, FLAT TOP, INJECTION PHYSICS BEAM, N/A, RAMP DOWN, SETUP, SQUEEZE
		if (lumifile_str.Contains("2022") || lumifile_str.Contains("2023")){
			if (sscanf(s.c_str(), "%d:%d,%d:%d,%d/%d/%d %d:%d:%d,STABLE BEAMS,%f,%f,%f,%f,%s",
						&rn, &fill, &numls, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &energy, &del, &rec, &avgpu, sfoo) != 15) { 
				//&rn, &fill, &ls, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &energy, &del, &rec, &avgpu, sfoo) != 15)
				skip = true;
			} 
		}
		else {
			if (sscanf(s.c_str(), "%d:%d,%d/%d/%d %d:%d:%d,%d,%d,%f,%f",
						&rn, &fill, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &numls, &ifoo, &del, &rec) != 12) {
				skip = true;
			}
		}
		//skip = true;

		if (debugevent)
			PrintInfo(Form("Run %d ls %d lumi %f/pb", rn, numls, rec * 1e-6), true);

		if (skip)
		{ // The user should know if this happens, since we can choose to use only STABLE BEAMS
			if (skip)
				PrintInfo(string("Skipping line (effects the recorded lumi):\n") + s, true);
			skip = false;
			continue;
		}

		//double lum = 0.0;
		//double lum2 = 0.0;
		if (lumifile_str.Contains("2022") || lumifile_str.Contains("2023")) {
			// Pass from /ub to /pb
			lum = rec * 1e-6;
			lum2 = del * 1e-6;
		} else {
			// Pass from /fb to /pb
			lum = rec * 1000.; //* 1e-6;
			lum2 = del * 1000.; //* 1e-6;
		}
		//double lum = rec * 1000.; //* 1e-6;
		//double lum2 = del * 1000.; //* 1e-6;

		//if (_lums[rn][numls] != 0)
		if (_lums[rn] != 0) {
			//Flag for luminoisty per lumisection instead of run
			//std::cout << "_lums already has a value for run " << rn << ", accumulating values." << std::endl;
			_lums[rn] += lum;
			_lums2[rn][numls] += lum2;
		} else {
			_lums[rn] = lum;
			_lums2[rn][numls] = lum2;
		}
		//if (_lums[rn] != 0) {
		//std::cout << "_lums already has a value for run " << rn << std::endl;
		//std::cout << "Exiting loop due to existing _lums value for run " << rn << std::endl;
		//return false;
		//}

		//if (_avgpu[rn][numls] != 0)
		//return false;
		// lumiCalc.py returns lumi in units of mub-1 (=>nb-1=>pb-1)

		//double lum = rec * 1000.; //* 1e-6;
		//double lum2 = del * 1000.; //* 1e-6;
		//Runs dictionary

		if (runNumbers.find(rn) == runNumbers.end()) {
			runNumbers.insert(rn);
		}

		std::vector<double> binEdges(runNumbers.begin(), runNumbers.end());
		binEdges.push_back(*runNumbers.rbegin() + 1);
		_runNumberBin = binEdges;

		if (lum == 0 and goodruns.find(rn) != goodruns.end() and (_json[rn][numls] == 1)) // The second condition had !jp::dojson or
			nolums.insert(pair<int, int>(rn, nls));

		//_avgpu[rn][numls] = avgpu * 69000. / 80000.; // brilcalc --minBiasXsec patch
		//_lums[rn][numls] = lum;

		// Not used anymore since is already used above. Nestor 23 July, 2024.
		//_lums[rn] = lum;
		//_lums2[rn][numls] = lum2;

		lumsum += lum;
		if (goodruns.find(rn) != goodruns.end()) // Apr 17
			lumsum_good += lum;
		if ((_json[rn][numls]))
			lumsum_json += lum;
		++nls;
		if (nls > 100000000)
			return false;
	}

	PrintInfo(Form("Called LoadLumi() with %s:\nLoaded %lu runs with %d lumi sections containing %f"
				" pb-1 of data,\n of which %f pb-1 is in good runs (%f%%)\nThis corresponds to %f"
				" hours of data-taking\nThe JSON file contains %f pb-1 (%f%%)",
				lumifile, _lums.size(), nls, lumsum, lumsum_good,
				100. * lumsum_good / lumsum, nls * secLS / 3600, lumsum_json, 100. * lumsum_json / lumsum),
			true);

	// Report any empty lumi section
	if (nolums.size() != 0)
	{
		PrintInfo(Form("Warning, found %lu non-normalizable LS:", nolums.size()), true);
		for (auto lumit = nolums.begin(); lumit != nolums.end(); ++lumit)
		{
			cout << " [" << lumit->first << "," << lumit->second;
			auto lumit2 = lumit;
			++lumit2;
			if (lumit2->first != lumit->first or lumit2->second != lumit->second + 1)
				cout << "]";
			else
			{
				for (int lumadd = 0; lumit2 != nolums.end() and lumit2->first == lumit->first and
						lumit2->second == lumit->second + lumadd + 1;
						++lumadd, ++lumit2)
				{
				};
				lumit = --lumit2;
				cout << "-" << lumit->second << "]";
			}
		} // for lumit
		cout << endl;
	} // nolums
	cout << "lumsum value: " << lumsum << endl;
	cout << endl;
	_lumsum = lumsum;
	cout << "_lumsum value: " << _lumsum << endl;
	//Unique run numbers

	/*  
	    for (const auto& number : runNumbers) {
	    std::cout << number << std::endl;
	    }

	    int run_example = 379866;
	    auto it = runNumbers.find(run_example);
	    if (it != runNumbers.end()) {
	    std::cout << run_example << " is included in runNumbers." << std::endl;
	    } else {
	    std::cout << run_example << " is not included in runNumbers." << std::endl;
	    }
	    auto it3 = std::find(_runNumberBin.begin(), _runNumberBin.end(), run_example);
	    if (it3 != _runNumberBin.end()) {
	    std::cout << "run_example is included in binEdges." << std::endl;
	    } else {
	    std::cout << "run_example is not included in binEdges." << std::endl;
	    }
	    */

	return true;
} // LoadLumi

/////////////////////////

void DijetHistosFill::Loop()
{
	//   In a ROOT session, you can do:
	//      root> .L DijetHistosFill.C
	//      root> DijetHistosFill t
	//      root> t.GetEntry(12); // Fill t data members with entry number 12
	//      root> t.Show();       // Show values of entry 12
	//      root> t.Show(16);     // Read and show values of entry 16
	//      root> t.Loop();       // Loop on all entries
	//

	//     This is the loop skeleton where:
	//    jentry is the global entry number in the chain
	//    ientry is the entry number in the current Tree
	//  Note that the argument to GetEntry must be:
	//    jentry for TChain::GetEntry
	//    ientry for TTree::GetEntry and TBranch::GetEntry
	//
	//       To read only selected branches, Insert statements like:
	// METHOD1:
	//    fChain->SetBranchStatus("*",0);  // disable all branches
	//    fChain->SetBranchStatus("branchname",1);  // activate branchname
	// METHOD2: replace line
	//    fChain->GetEntry(jentry);       //read all branches
	// by  b_branchname->GetEntry(ientry); //read only this branch
	if (fChain == 0)
		return;

	// ROOT.EnableImplicitMT(); // From Nico on Skype, to parallelize processing

	TStopwatch fulltime, laptime;
	fulltime.Start();
	TDatime bgn;
	TDatime start_time;
	start_time.Set();
	int nlap(0);

	fChain->SetBranchStatus("*", 0);

	// if (debug)
	cout << "Setting branch status for "
		<< (isMC ? (isMG ? "MC (MG)" : "MC (Flat)") : (isZB ? "DATA (ZB)" : "DATA"))
		<< " and " << (isRun2 ? "Run2" : "Run3")
		<< " (isRun2=" << isRun2 << ", isRun3=" << isRun3 << ")"
		<< endl
		<< flush;

	if (isMC)
		fChain->SetBranchStatus("genWeight", 1);
	if (isMC)
		fChain->SetBranchStatus("Generator_binvar", 1); // pThat in Pythia8
	if (isMC && !(isMG && isRun3))
		fChain->SetBranchStatus("Pileup_pthatmax", 1);

	if (isMC && (reweightPU || doPU_per_trigger || do_PUProfiles))
	{
		fChain->SetBranchStatus("Pileup_nTrueInt", 1);
		fChain->SetBranchStatus("Pileup_nPU", 1);
		//fChain->SetBranchStatus("PV_npvs", 1);
		//fChain->SetBranchStatus("PV_npvsGood", 1);
	}

	if (do_PUProfiles){
                fChain->SetBranchStatus("PV_npvs", 1);
                fChain->SetBranchStatus("PV_npvsGood", 1);	
	}

	if (isMC && (smearJets || doMCtruth))
	{
		cout << "Adding branches for GenJets ("
			<< (smearJets ? " smearJets" : "")
			<< (doMCtruth ? " doMCtruth" : "") << ")" << endl;
		fChain->SetBranchStatus("Jet_genJetIdx", 1);
		fChain->SetBranchStatus("nGenJet", 1);
		fChain->SetBranchStatus("GenJet_pt", 1);
		fChain->SetBranchStatus("GenJet_eta", 1);
		fChain->SetBranchStatus("GenJet_phi", 1);
		fChain->SetBranchStatus("GenJet_mass", 1);

		if (doMCtruth)
		{
			fChain->SetBranchStatus("GenVtx_z", 1);
			fChain->SetBranchStatus("PV_z", 1);
		}

		// At the value of _seed: the old question - should the seed of a rng be random itself?
		// Here we prefer stability, but the user can vary the seed if necessary. Moreover, https://xkcd.com/221/
		_seed = 4;
		_mersennetwister = std::mt19937(_seed);
	}

	if (isMG)
		fChain->SetBranchStatus("LHE_HT", 1); // HT in MadGraph

	fChain->SetBranchStatus("run", 1);
	fChain->SetBranchStatus("luminosityBlock", 1);
	fChain->SetBranchStatus("event", 1);
	// fChain->SetBranchStatus("Rho_fixedGridRhoAll",1);
	if (isRun2)
		fChain->SetBranchStatus("fixedGridRhoFastjetAll", 1);
	if (isRun3)
		//if (isRun3 || isMG )
		fChain->SetBranchStatus("Rho_fixedGridRhoFastjetAll", 1);
	//if (!TString(dataset.c_str()).Contains("2024") || !TString(dataset.c_str()).Contains("Winter24MGV14"))
	//fChain->SetBranchStatus("L1_UnprefireableEvent", 1);
	//fChain->SetBranchStatus("L1_UnprefireableEvent", 1);

	// Listing of available triggers
	vector<string> vtrg = {
		"HLT_PFJet40",
		"HLT_PFJet60",
		"HLT_PFJet80",
		"HLT_PFJet140",
		"HLT_PFJet200",
		"HLT_PFJet260",
		"HLT_PFJet320",
		"HLT_PFJet400",
		"HLT_PFJet450",
		"HLT_PFJet500",
		"HLT_PFJetFwd40", //
		"HLT_PFJetFwd60",
		"HLT_PFJetFwd80",
		"HLT_PFJetFwd140",
		"HLT_PFJetFwd200",
		"HLT_PFJetFwd260",
		"HLT_PFJetFwd320",
		"HLT_PFJetFwd400",
		"HLT_PFJetFwd450",
		"HLT_PFJetFwd500", //
		"HLT_DiPFJetAve40",
		"HLT_DiPFJetAve60",
		"HLT_DiPFJetAve80",
		"HLT_DiPFJetAve140",
		"HLT_DiPFJetAve200",
		"HLT_DiPFJetAve260",
		"HLT_DiPFJetAve320",
		"HLT_DiPFJetAve400",
		"HLT_DiPFJetAve500",
		"HLT_DiPFJetAve60_HFJEC",
		"HLT_DiPFJetAve80_HFJEC",
		"HLT_DiPFJetAve100_HFJEC",
		"HLT_DiPFJetAve160_HFJEC",
		"HLT_DiPFJetAve220_HFJEC",
		"HLT_DiPFJetAve300_HFJEC"};

	if (isZB)
		vtrg.insert(vtrg.begin(), "HLT_ZeroBias");

	if (isRun2 > 2)
		vtrg.push_back("HLT_PFJet550");
	//}

	// vtrg.push_back("HLT_PFJetFwd15");
	// vtrg.push_back("HLT_PFJetFwd25");
if (isRun2 > 2)
{ // && dataset!="UL2017B") {
	vtrg.push_back("HLT_PFJetFwd40");
	vtrg.push_back("HLT_PFJetFwd60");
	vtrg.push_back("HLT_PFJetFwd80");
	vtrg.push_back("HLT_PFJetFwd140");
	vtrg.push_back("HLT_PFJetFwd200");
	vtrg.push_back("HLT_PFJetFwd260");
	vtrg.push_back("HLT_PFJetFwd320");
	vtrg.push_back("HLT_PFJetFwd400");
	vtrg.push_back("HLT_PFJetFwd450");
	vtrg.push_back("HLT_PFJetFwd500");
}

if (doMCtrigOnly && isMC) //Set doMCtrigOnly = true and comment vtrg.clear(); to see the list of triggers
{
	vtrg.clear();
	vtrg.push_back("HLT_MC");
}

if (isZB && !isMC)
{
	vtrg.clear(); // no jet triggers from ZeroBias PD
	vtrg.push_back("HLT_ZeroBias");
}

int ntrg = vtrg.size();

for (int i = 0; i != ntrg; ++i)
{
	if (vtrg[i] != "HLT_MC")
		fChain->SetBranchStatus(vtrg[i].c_str(), 1); 
	if (mtrg[vtrg[i]] == 0)
	{
		cout << "Missing branch info for " << vtrg[i] << endl
			<< flush;
	}
	assert(mtrg[vtrg[i]] != 0);
}

fChain->SetBranchStatus("nJet", 1);
fChain->SetBranchStatus("Jet_btagPNetQvG", 1);
fChain->SetBranchStatus("Jet_btagUParTAK4QvG", 1);
fChain->SetBranchStatus("Jet_pt", 1);
fChain->SetBranchStatus("Jet_eta", 1);
fChain->SetBranchStatus("Jet_phi", 1);
fChain->SetBranchStatus("Jet_mass", 1);
fChain->SetBranchStatus("Jet_jetId", 1);

fChain->SetBranchStatus("Jet_rawFactor", 1);
if (isRun2)
	fChain->SetBranchStatus("Jet_area", 1);

	// bool doPFComposition = true;
if (doPFComposition)
{
	fChain->SetBranchStatus("Jet_chHEF", 1);  // h+
	fChain->SetBranchStatus("Jet_neHEF", 1);  // h0
	fChain->SetBranchStatus("Jet_neEmEF", 1); // gamma
	fChain->SetBranchStatus("Jet_chEmEF", 1); // e
	fChain->SetBranchStatus("Jet_muEF", 1);   // mu
	// fChain->SetBranchStatus("Jet_hfEmEF",1); // HFe
	// fChain->SetBranchStatus("Jet_hfHEF",1);  // HFh
}

double Jet_l1rcFactor[nJetMax]; // For L1L2L3-RC type-I MET
if (isRun2)
{
	// raw chs PF MET
	fChain->SetBranchStatus("ChsMET_pt", 1);
	fChain->SetBranchStatus("ChsMET_phi", 1);
}
else
{
	// fChain->SetBranchStatus("PuppiMET_pt",1);
	// fChain->SetBranchStatus("PuppiMET_phi",1);
	fChain->SetBranchStatus("RawPuppiMET_pt", 1);
	fChain->SetBranchStatus("RawPuppiMET_sumEt", 1);
	fChain->SetBranchStatus("RawPuppiMET_phi", 1);
	//fChain->SetBranchStatus("MET_pt", 1);
	//fChain->SetBranchStatus("MET_sumEt", 1);
}

fChain->SetBranchStatus("Flag_METFilters", 1);
if (isRun2 || isRun3) //April 2, 2024: Same filters for UL Run2 and Run3
{
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/MissingETOptionalFiltersRun2#Run_3_recommendations
	fChain->SetBranchStatus("Flag_goodVertices", 1);
	fChain->SetBranchStatus("Flag_globalSuperTightHalo2016Filter", 1);
	fChain->SetBranchStatus("Flag_EcalDeadCellTriggerPrimitiveFilter", 1);
	fChain->SetBranchStatus("Flag_BadPFMuonFilter", 1);
	fChain->SetBranchStatus("Flag_BadPFMuonDzFilter", 1);
	fChain->SetBranchStatus("Flag_hfNoisyHitsFilter", 1);
	fChain->SetBranchStatus("Flag_eeBadScFilter", 1);
	fChain->SetBranchStatus("Flag_ecalBadCalibFilter", 1);
}

// Trigger studies => TrigObjAK4 later (fixed now)
bool doTriggerMatch = false;
nTrigObjJMEAK4 = 0; // turn off
if (doTriggerMatch)
{
	// https://github.com/cms-sw/cmssw/blob/CMSSW_12_4_8/PhysicsTools/NanoAOD/python/triggerObjects_cff.py#L136-L180
	fChain->SetBranchStatus("nTrigObjJMEAK4", 1);
	fChain->SetBranchStatus("TrigObjJMEAK4_pt", 1);
	fChain->SetBranchStatus("TrigObjJMEAK4_eta", 1);
	fChain->SetBranchStatus("TrigObjJMEAK4_phi", 1);
}

// List reference pT and abseta thresholds for triggers
mt["HLT_MC"] = range{10, 3000, 0, 5.2};
mt["HLT_ZeroBias"] = range{10, 3000, 0, 5.2};

mt["HLT_DiPFJetAve40"] = range{40, 85, 0, 5.2};
mt["HLT_DiPFJetAve60"] = range{85, 100, 0, 5.2};
mt["HLT_DiPFJetAve80"] = range{100, 155, 0, 5.2};
mt["HLT_DiPFJetAve140"] = range{155, 210, 0, 5.2};
mt["HLT_DiPFJetAve200"] = range{210, 300, 0, 5.2};
mt["HLT_DiPFJetAve260"] = range{300, 400, 0, 5.2};
mt["HLT_DiPFJetAve320"] = range{400, 500, 0, 5.2};
mt["HLT_DiPFJetAve400"] = range{500, 600, 0, 5.2};
mt["HLT_DiPFJetAve500"] = range{600, 6500, 0, 5.2};

// 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191,
double fwdeta = 3.139;  // was 2.853. 80% (100%) on negative (positive) side
double fwdeta0 = 2.964; // 2.853; // 40 and 260 up

double fwdetad = 2.853;

mt["HLT_DiPFJetAve60_HFJEC"] = range{85, 100, fwdeta, 5.2};
mt["HLT_DiPFJetAve80_HFJEC"] = range{100, 125, fwdeta, 5.2};
mt["HLT_DiPFJetAve100_HFJEC"] = range{125, 180, fwdeta, 5.2};
mt["HLT_DiPFJetAve160_HFJEC"] = range{180, 250, fwdeta, 5.2};
mt["HLT_DiPFJetAve220_HFJEC"] = range{250, 350, fwdeta0, 5.2};
mt["HLT_DiPFJetAve300_HFJEC"] = range{350, 6500, fwdeta0, 5.2};

mt["HLT_PFJet40"] = range{40, 85, 0, 5.2};
mt["HLT_PFJet60"] = range{85, 100, 0, 5.2};
mt["HLT_PFJet80"] = range{100, 155, 0, 5.2};
mt["HLT_PFJet140"] = range{155, 210, 0, 5.2};
mt["HLT_PFJet200"] = range{210, 300, 0, 5.2};
mt["HLT_PFJet260"] = range{300, 400, 0, 5.2};
mt["HLT_PFJet320"] = range{400, 500, 0, 5.2};
mt["HLT_PFJet400"] = range{500, 600, 0, 5.2};
mt["HLT_PFJet450"] = range{500, 600, 0, 5.2};
mt["HLT_PFJet500"] = range{600, 6500, 0, 5.2};
mt["HLT_PFJet550"] = range{700, 6500, 0, 5.2};

mt["HLT_PFJetFwd40"] = range{40, 85, fwdeta0, 5.2};
mt["HLT_PFJetFwd60"] = range{85, 100, fwdeta, 5.2};
mt["HLT_PFJetFwd80"] = range{100, 155, fwdeta, 5.2};
mt["HLT_PFJetFwd140"] = range{155, 210, fwdeta, 5.2};
mt["HLT_PFJetFwd200"] = range{210, 300, fwdeta0, 5.2};
mt["HLT_PFJetFwd260"] = range{300, 400, fwdeta0, 5.2};
mt["HLT_PFJetFwd320"] = range{400, 500, fwdeta0, 5.2};
mt["HLT_PFJetFwd400"] = range{500, 600, fwdeta0, 5.2};
mt["HLT_PFJetFwd450"] = range{500, 600, fwdeta0, 5.2}; // x
mt["HLT_PFJetFwd500"] = range{600, 6500, fwdeta0, 5.2};

// For jetrate vs runs
mi["HLT_ZeroBias"] = range{10,  49,  0, 5.2};
//mi["HLT_MC"]       = range{15,6500,  0, 5.2};
mi["HLT_PFJet40"]  = range{49,  84,  0, fwdeta0}; //Ref number from vtrg: 0
mi["HLT_PFJet60"]  = range{84,  114, 0, fwdeta};  // 1
mi["HLT_PFJet80"]  = range{114, 196, 0, fwdeta}; // 2
mi["HLT_PFJet140"] = range{196, 272, 0, fwdeta}; // 3
mi["HLT_PFJet200"] = range{272, 330, 0, fwdeta0}; // 4
mi["HLT_PFJet260"] = range{330, 395, 0, fwdeta0}; // 5
mi["HLT_PFJet320"] = range{395, 468, 0, fwdeta0}; // 6
mi["HLT_PFJet400"] = range{468, 548, 0, fwdeta0}; // 7
mi["HLT_PFJet450"] = range{548, 686, 0, fwdeta0}; // 8
mi["HLT_PFJet500"] = range{686,6500, 0, fwdeta0}; // 9
//mi["HLT_PFJet550"] = range{700,3000, 0, fwdeta0};

mi["HLT_PFJetFwd40"]  = range{49,  84,  fwdeta0, 5.2}; //  10
mi["HLT_PFJetFwd60"]  = range{84,  114, fwdeta, 5.2}; // 11
mi["HLT_PFJetFwd80"]  = range{114, 196, fwdeta, 5.2}; // 12
mi["HLT_PFJetFwd140"] = range{196, 272, fwdeta, 5.2}; // 13
mi["HLT_PFJetFwd200"] = range{272, 330, fwdeta0, 5.2}; // 14
mi["HLT_PFJetFwd260"] = range{330, 395, fwdeta0, 5.2}; // 15
mi["HLT_PFJetFwd320"] = range{395, 468, fwdeta0, 5.2}; // 16
mi["HLT_PFJetFwd400"] = range{468, 548, fwdeta0, 5.2}; // 16
mi["HLT_PFJetFwd450"] = range{548, 686, fwdeta0, 5.2}; // 17
mi["HLT_PFJetFwd500"] = range{686,6500, fwdeta0, 5.2}; // 18
///


// Triggers from the DijetHistosCombine.C 
// dijet 
md["HLT_ZeroBias"]      = range{15,  40,  0, 5.2};
md["HLT_DiPFJetAve40"]  = range{40,  85,  0, 5.2};
md["HLT_DiPFJetAve60"]  = range{85,  100, 0, fwdeta};
md["HLT_DiPFJetAve80"]  = range{100, 155, 0, fwdeta};
md["HLT_DiPFJetAve140"] = range{155, 250, 0, fwdeta};
md["HLT_DiPFJetAve200"] = range{250, 300, 0, fwdeta0}; // 210->250
md["HLT_DiPFJetAve260"] = range{300, 400, 0, fwdeta0};
md["HLT_DiPFJetAve320"] = range{400, 500, 0, fwdeta0};
md["HLT_DiPFJetAve400"] = range{500, 600, 0, fwdeta0};
md["HLT_DiPFJetAve500"] = range{600,3000, 0, fwdeta0};

md["HLT_DiPFJetAve60_HFJEC"]  = range{85,  100, fwdeta, 5.2};
md["HLT_DiPFJetAve80_HFJEC"]  = range{100, 125, fwdeta, 5.2};
md["HLT_DiPFJetAve100_HFJEC"] = range{125, 180, fwdeta, 5.2};
md["HLT_DiPFJetAve160_HFJEC"] = range{180, 250, fwdeta, 5.2};
md["HLT_DiPFJetAve220_HFJEC"] = range{250, 350, fwdeta0, 5.2};
md["HLT_DiPFJetAve300_HFJEC"] = range{350,3000, fwdeta0, 5.2};

// dijet2
md2["HLT_ZeroBias"]      = range{15,  59,  0, 5.2};
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

// dijet2 -> probe binning
md2pf["HLT_ZeroBias"] = range{15,  59,  0, 5.2};
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

// dijet2 -> tag binning
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


////
//
//
if (doPU_per_trigger){
  get_PU_hist("PUWeight2024F");
  //print_PU_hist_map();
}

if (debug)
	cout << "Setting up JEC corrector" << endl
	<< flush;

	// Redo JEC
	// NB: could implement time dependence as in jetphys/IOV.h
	FactorizedJetCorrector *jec(0), *jecl1rc(0), *jersfvspt(0);
	string jerpath(""), jerpathsf("");
	// jec = getFJC("","Winter22Run3_V1_MC_L2Relative","","");
	// if (isRun2==0) {
	// jec = getFJC("","Winter22Run3_V1_MC_L2Relative",
	//		  isMC ? "":"Winter22Run3_RunC_V2_DATA_L2L3Residual_AK4PFPuppi");
	// }
	//  2016APV (BCD, EF)

	TH1D *pileupRatio = new TH1D("puRatio", "PURatio;;Ratio", 99, 0, 100);

	if (dataset == "UL2016APVMG")
{
	jec = getFJC("Summer19UL16APV_V7_MC_L1FastJet_AK4PFchs",
			"Summer19UL16APV_V7_MC_L2Relative_AK4PFchs", "");
	jecl1rc = getFJC("Summer19UL16APV_V7_MC_L1RC_AK4PFchs", "", "");
	jerpath = "JRDatabase/textFiles/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_PtResolution_AK4PFchs.txt";
	jerpathsf = "JRDatabase/textFiles/Summer20UL16APV_JRV3_MC/Summer20UL16APV_JRV3_MC_SF_AK4PFchs.txt";
	jersfvspt = getFJC("", "Summer20UL2016APV_ZB_v26c_JRV3_MC_SF_AK4PFchs", "");
}
if (dataset == "UL2016BCD" || dataset == "UL2016BCD_ZB")
{
	jec = getFJC("Summer19UL16APV_RunBCD_V7_DATA_L1FastJet_AK4PFchs",
			"Summer19UL16APV_RunBCD_V7_DATA_L2Relative_AK4PFchs",
			"Summer19UL16APV_RunBCD_V7_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL16APV_RunBCD_V7_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2016EF" || dataset == "UL2016EF_ZB")
{
	jec = getFJC("Summer19UL16APV_RunEF_V7_DATA_L1FastJet_AK4PFchs",
			"Summer19UL16APV_RunEF_V7_DATA_L2Relative_AK4PFchs",
			"Summer19UL16APV_RunEF_V7_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL16APV_RunEF_V7_DATA_L1RC_AK4PFchs", "", "");
}
// 2016 non-APV (GH)
if (dataset == "UL2016MG" || dataset == "UL2016Flat")
{
	jec = getFJC("Summer19UL16_V7_MC_L1FastJet_AK4PFchs",
			"Summer19UL16_V7_MC_L2Relative_AK4PFchs", "");
	jecl1rc = getFJC("Summer19UL16_V7_MC_L1RC_AK4PFchs", "", "");
	// jec = getFJC("Summer20UL16_V1_MC_L1FastJet_AK4PFchs",
	//		  "Summer20UL16_V1_MC_L2Relative_AK4PFchs","");
	// jecl1rc = getFJC("Summer20UL16_V1_MC_L1RC_AK4PFchs","","");
	jerpath = "JRDatabase/textFiles/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_PtResolution_AK4PFchs.txt";
	jerpathsf = "JRDatabase/textFiles/Summer20UL16_JRV3_MC/Summer20UL16_JRV3_MC_SF_AK4PFchs.txt";
	jersfvspt = getFJC("", "Summer20UL2016GH_ZB_v26c_JRV3_MC_SF_AK4PFchs", "");
}
if (dataset == "UL2016GH" || dataset == "UL2016GH_ZB")
{
	jec = getFJC("Summer19UL16_RunFGH_V7_DATA_L1FastJet_AK4PFchs",
			"Summer19UL16_RunFGH_V7_DATA_L2Relative_AK4PFchs",
			"Summer19UL16_RunFGH_V7_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL16_RunFGH_V7_DATA_L1RC_AK4PFchs", "", "");
	// jec = getFJC("Summer20UL16_RunGH_V1_DATA_L1FastJet_AK4PFchs",
	//"Summer20UL16_RunGH_V1_DATA_L2Relative_AK4PFchs",
	//"Summer20UL16_RunGH_V1_DATA_L2L3Residual_AK4PFchs");
	//"Summer19UL16_RunFGH_V7_DATA_L2L3Residual_AK4PFchs");
	// jecl1rc = getFJC("Summer20UL16_RunGH_V1_DATA_L1RC_AK4PFchs","","");
}
// 2017
if (dataset == "UL2017MG")
{
	jec = getFJC("Summer19UL17_V6_MC_L1FastJet_AK4PFchs",
			"Summer19UL17_V6_MC_L2Relative_AK4PFchs", "");
	jecl1rc = getFJC("Summer19UL17_V6_MC_L1RC_AK4PFchs", "", "");
	jerpath = "JRDatabase/textFiles/Summer19UL17_JRV3_MC/Summer19UL17_JRV3_MC_PtResolution_AK4PFchs.txt";
	jerpathsf = "JRDatabase/textFiles/Summer19UL17_JRV3_MC/Summer19UL17_JRV3_MC_SF_AK4PFchs.txt";
	jersfvspt = getFJC("", "Summer20UL2017_ZB_v26c_JRV3_MC_SF_AK4PFchs", "");
}
if (dataset == "UL2017B" || dataset == "UL2017B_ZB")
{
	jec = getFJC("Summer19UL17_RunB_V6_DATA_L1FastJet_AK4PFchs",
			"Summer19UL17_RunB_V6_DATA_L2Relative_AK4PFchs",
			"Summer19UL17_RunB_V6_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL17_RunB_V6_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2017C" || dataset == "UL2017C_ZB")
{
	jec = getFJC("Summer19UL17_RunC_V6_DATA_L1FastJet_AK4PFchs",
			"Summer19UL17_RunC_V6_DATA_L2Relative_AK4PFchs",
			"Summer19UL17_RunC_V6_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL17_RunC_V6_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2017D" || dataset == "UL2017D_ZB")
{
	jec = getFJC("Summer19UL17_RunD_V6_DATA_L1FastJet_AK4PFchs",
			"Summer19UL17_RunD_V6_DATA_L2Relative_AK4PFchs",
			"Summer19UL17_RunD_V6_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL17_RunD_V6_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2017E" || dataset == "UL2017E_ZB")
{
	jec = getFJC("Summer19UL17_RunE_V6_DATA_L1FastJet_AK4PFchs",
			"Summer19UL17_RunE_V6_DATA_L2Relative_AK4PFchs",
			"Summer19UL17_RunE_V6_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL17_RunE_V6_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2017F" || dataset == "UL2017F_ZB")
{
	jec = getFJC("Summer19UL17_RunF_V6_DATA_L1FastJet_AK4PFchs",
			"Summer19UL17_RunF_V6_DATA_L2Relative_AK4PFchs",
			"Summer19UL17_RunF_V6_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL17_RunF_V6_DATA_L1RC_AK4PFchs", "", "");
}
// 2018
if (dataset == "UL2018MG" || TString(dataset.c_str()).Contains("UL2018MG") || TString(dataset.c_str()).Contains("UL2018MC"))
{
	jec = getFJC("Summer19UL18_V5_MC_L1FastJet_AK4PFchs",
			"Summer19UL18_V5_MC_L2Relative_AK4PFchs", "");
	jecl1rc = getFJC("Summer19UL18_V5_MC_L1RC_AK4PFchs", "", "");
	//jerpath = "JRDatabase/textFiles/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_PtResolution_AK4PFchs.txt";
	//jerpathsf = "JRDatabase/textFiles/Summer19UL18_JRV2_MC/Summer19UL18_JRV2_MC_SF_AK4PFchs.txt";
	//jersfvspt = getFJC("", "Summer20UL2018_ZB_v26c_JRV3_MC_SF_AK4PFchs", "");
	jerpath = "";
	jerpathsf = "";
	//jersfvspt = "";
}
if (dataset == "UL2018A" || dataset == "UL2018A_ZB")
{
	jec = getFJC("Summer19UL18_RunA_V5_DATA_L1FastJet_AK4PFchs",
			"Summer19UL18_RunA_V5_DATA_L2Relative_AK4PFchs",
			"Summer19UL18_RunA_V5_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL18_RunA_V5_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2018B" || dataset == "UL2018B_ZB")
{
	jec = getFJC("Summer19UL18_RunB_V5_DATA_L1FastJet_AK4PFchs",
			"Summer19UL18_RunB_V5_DATA_L2Relative_AK4PFchs",
			"Summer19UL18_RunB_V5_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL18_RunB_V5_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2018C" || dataset == "UL2018C_ZB")
{
	jec = getFJC("Summer19UL18_RunC_V5_DATA_L1FastJet_AK4PFchs",
			"Summer19UL18_RunC_V5_DATA_L2Relative_AK4PFchs",
			"Summer19UL18_RunC_V5_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL18_RunC_V5_DATA_L1RC_AK4PFchs", "", "");
}
if (dataset == "UL2018D" || dataset == "UL2018D_ZB" ||
		dataset == "UL2018D1" || dataset == "UL2018D2")
{
	jec = getFJC("Summer19UL18_RunD_V5_DATA_L1FastJet_AK4PFchs",
			"Summer19UL18_RunD_V5_DATA_L2Relative_AK4PFchs",
			"Summer19UL18_RunD_V5_DATA_L2L3Residual_AK4PFchs");
	jecl1rc = getFJC("Summer19UL18_RunD_V5_DATA_L1RC_AK4PFchs", "", "");
}

// 2022
//  Align JECs with
//  https://indico.cern.ch/event/1335203/#7-update-on-l2res-for-2022-rer
if (dataset == "2022C" || dataset == "2022C_ZB" || dataset == "2022C_prompt" || dataset == "2022C_ZB_prompt")
{
	jec = getFJC("",                                       // Winter22Run3_RunC_V2_DATA_L1FastJet_AK4PFPuppi",
			//"Winter22Run3_RunC_V2_DATA_L2Relative_AK4PFPuppi",
			"Summer22Run3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			// v33 and prev "Summer22_RunCD_V2_MPF_L2Residual_AK4PFPuppi");
			// "Run22CD-22Sep2023_DATA_L2L3Residual_AK4PFPuppi"
	    "Summer22-22Sep2023_Run2022CD_V3_DATA_L2L3Residual_AK4PFPuppi");
	//"");//"Winter22Run3_RunC_V2_DATA_L2L3Residual_AK4PFPuppi");
}
if (dataset == "2022D" || dataset == "2022D_ZB" || dataset == "2022D_prompt" || dataset == "2022D_ZB_prompt")
{
	jec = getFJC("",                                       // Winter22Run3_RunD_V2_DATA_L1FastJet_AK4PFPuppi",
			//"Winter22Run3_RunD_V2_DATA_L2Relative_AK4PFPuppi",
			"Summer22Run3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			// v33 and prev "Summer22_RunCD_V2_MPF_L2Residual_AK4PFPuppi");
			//"Run22CD-22Sep2023_DATA_L2L3Residual_AK4PFPuppi"
	    "Summer22-22Sep2023_Run2022CD_V3_DATA_L2L3Residual_AK4PFPuppi");
	//"");//"Winter22Run3_RunD_V2_DATA_L2L3Residual_AK4PFPuppi");
}
if (dataset == "2022E" || dataset == "2022E_ZB")
{
	jec = getFJC("",                                             // Summer22EEPrompt22_RunF_V1_DATA_L1FastJet_AK4PFPuppi",
			//"Summer22EEPrompt22_RunF_V1_DATA_L2Relative_AK4PFPuppi",
			"Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			// v33 and prev "Summer22EE_RunE_V2_MPF_L2Residual_AK4PFPuppi");
			// "Run22E-22Sep2023_DATA_L2L3Residual_AK4PFPuppi"
	    "Summer22EE-22Sep2023_Run2022E_V3_DATA_L2L3Residual_AK4PFPuppi");
	//"Summer22EEPrompt22_RunE_V2_L2Residual_AK4PFPuppi");
	//"");//"Summer22EEPrompt22_RunF_V1_DATA_L2L3Residual_AK4PFPuppi");
}
// if (dataset=="2022F" || dataset=="2022F_ZB") {
if (TString(dataset.c_str()).Contains("2022F"))
{
	jec = getFJC("",                                             // Summer22EEPrompt22_RunF_V1_DATA_L1FastJet_AK4PFPuppi",
			//"Summer22EEPrompt22_RunF_V1_DATA_L2Relative_AK4PFPuppi",
			"Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			//"Summer22EEPrompt22_RunF_V2_L2Residual_AK4PFPuppi"
			// "Run22F-Prompt_DATA_L2L3Residual_AK4PFPuppi"
			"Summer22EEPrompt22_Run2022F_V3_DATA_L2L3Residual_AK4PFPuppi");
	//"");//"Summer22EEPrompt22_RunF_V1_DATA_L2L3Residual_AK4PFPuppi");
}
if (dataset == "2022G" || dataset == "2022G_ZB")
{
	jec = getFJC("",                                             // Summer22EEPrompt22_RunG_V1_DATA_L1FastJet_AK4PFPuppi",
			//"Summer22EEPrompt22_RunG_V1_DATA_L2Relative_AK4PFPuppi",
			"Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			// "Summer22EEPrompt22_RunG_V2_L2Residual_AK4PFPuppi"
			// "Run22G-Prompt_DATA_L2L3Residual_AK4PFPuppi"
			"Summer22EEPrompt22_Run2022G_V3_DATA_L2L3Residual_AK4PFPuppi");
	//"");//"Summer22EEPrompt22_RunG_V1_DATA_L2L3Residual_AK4PFPuppi");
}

// 22/23 MC
if (dataset == "Summer22" ||
		dataset == "Summer22Flat" ||
		TString(dataset.c_str()).Contains("Summer22MC") ||
		TString(dataset.c_str()).Contains("Summer22MG"))
{
	jec = getFJC("",                                       // Winter22Run3_V2_MC_L1FastJet_AK4PFPuppi",
			//"Winter22Run3_V2_MC_L2Relative_AK4PFPuppi",
			"Summer22Run3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			"");                                      // Winter22Run3_V2_MC_L2L3Residual_AK4PFPuppi");
	//jerpath = "CondFormats/JetMETObjects/data/Summer22_V1_NSCP_MC_PtResolution_ak4puppi.txt";
	//jerpathsf = "CondFormats/JetMETObjects/data/Summer22EERun3_V1_MC_SF_AK4PFPuppi.txt"; // Same as Summer22EE, is ok
	jerpath = "";
	jerpathsf = "";
	useJERSFvsPt = false;
}
if (dataset == "Summer22EE" ||
		dataset == "Summer22EEFlat" ||
		TString(dataset.c_str()).Contains("Summer22EEMG"))
{
	jec = getFJC("",                                             // Summer22EEPrompt22_V1_MC_L1FastJet_AK4PFPuppi",
			//"Summer22EEPrompt22_V1_MC_L2Relative_AK4PFPuppi",
			"Summer22EEVetoRun3_V1_MC_L2Relative_AK4PUPPI", // Mikel
			"");                                            // Summer22EEPrompt22_V1_MC_L2L3Residual_AK4PFPuppi");
	jerpath = "CondFormats/JetMETObjects/data/Summer22EEVetoRun3_V1_NSCP_MC_PtResolution_ak4puppi.txt";
	jerpathsf = "CondFormats/JetMETObjects/data/Summer22EERun3_V1_MC_SF_AK4PFPuppi.txt";
	//jerpath = "";
	//jerpathsf = "";
	useJERSFvsPt = false;
}
if (dataset == "Summer23" ||
		dataset == "Summer23MCFlat" || dataset == "Summer23MG" || TString(dataset.c_str()).Contains("Summer23MC") ||
		dataset == "Summer23MCBPixFlat" || dataset == "Summer23BPIXMG" || TString(dataset.c_str()).Contains("Summer23"))
{
	if (TString(dataset.c_str()).Contains("Summer23MGBPix") || TString(dataset.c_str()).Contains("Summer23MCBPixFlat") || TString(dataset.c_str()).Contains("Summer23MCBPix")) {
		jec = getFJC("",
				"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI",
				"");
		//jerpathsf = "";
		jersfvspt = getFJC("", "", "");
		jerpathsf = "CondFormats/JetMETObjects/data/Summer23_2023D_JRV1_MC_SF_AK4PFPuppi.txt";
		//jersfvspt = getFJC("", "Summer23_2023D_JRV1_MC_SF_AK4PFPuppi", "");
		//jersfvspt = getFJC("", "Summer23_2023D_JRV2_MC_SF_AK4PFPuppi", "");
	} else {
		jec = getFJC("", 
				"Summer23Run3_V1_MC_L2Relative_AK4PUPPI",
				"");
		//jerpathsf = "";
		jersfvspt = getFJC("", "", "");
		jerpathsf = "CondFormats/JetMETObjects/data/Summer23_2023Cv123_JRV1_MC_SF_AK4PFPuppi";
		//jersfvspt = getFJC("", "Summer23_2023Cv123_JRV1_MC_SF_AK4PFPuppi", "");
		//jersfvspt = getFJC("", "Summer23_2023Cv4_JRV1_MC_SF_AK4PFPuppi", "");
		// Resolution SF version 2: https://indico.cern.ch/event/1399194/
		// April 3, 2024
		//jersfvspt = getFJC("", "Summer23_2023Cv123_JRV2_MC_SF_AK4PFPuppi", "");
		//jersfvspt = getFJC("", "Summer23_2023Cv4_JRV2_MC_SF_AK4PFPuppi", "");
	}
	//jec = getFJC("", // Winter23Prompt23_V2_MC_L1FastJet_AK4PFPuppi",
	//             "Winter23Prompt23_V2_MC_L2Relative_AK4PFPuppi",
	//             "");                                                                                   // Winter23Prompt23_V2_MC_L2L3Residual_AK4PFPuppi");
	//jerpath = "";
	//jerpath = "CondFormats/JetMETObjects/data/Summer22EEVetoRun3_V1_NSCP_MC_PtResolution_ak4puppi.txt"; // Same as Summer22EE, until updated
	jerpath = "CondFormats/JetMETObjects/data/Summer23BPixPrompt23_RunD_JRV1_MC_PtResolution_AK4PFPuppi.txt";
	useJERSFvsPt = false; //Nestor, 24 July, 2024.

	if (reweightPU)
	{
		if (TString(dataset.c_str()).Contains("Summer23MGBPix")) {
			TFile f("luminosityscripts/PUWeights/Summer23BPix_PUWeight.root");
			pileupRatio = (TH1D *)f.Get("pileup");
			pileupRatio->SetDirectory(0);
			// Print mean, min weight, max weight
			cout << "Pileup ratio mean = " << pileupRatio->GetMean() << endl;
			cout << "Pileup ratio min = " << pileupRatio->GetMinimum() << endl;
			cout << "Pileup ratio max = " << pileupRatio->GetMaximum() << endl;

		} else {
			TFile f("luminosityscripts/PUWeights/Summer23_PUWeight.root");
			pileupRatio = (TH1D *)f.Get("pileup");
			pileupRatio->SetDirectory(0);
			// Print mean, min weight, max weight
			cout << "Pileup ratio mean = " << pileupRatio->GetMean() << endl;
			cout << "Pileup ratio min = " << pileupRatio->GetMinimum() << endl;
			cout << "Pileup ratio max = " << pileupRatio->GetMaximum() << endl;

		}
	}
}

// 2023
// if (dataset=="2023B" || dataset=="2023B_ZB") {
// jec = getFJC("Winter23Prompt23_RunC_V2_DATA_L1FastJet_AK4PFPuppi",
//		  "Winter23Prompt23_RunC_V2_DATA_L2Relative_AK4PFPuppi",
//		  "Winter23Prompt23_RunC_V2_DATA_L2L3Residual_AK4PFPuppi");
// }

if (dataset == "2023B" || dataset == "2023B_ZB" || dataset == "2023BCv123" || 
		dataset == "2023BCv123_ZB" || dataset == "2023Cv123" || dataset == "2023Cv123_ZB" ||
		dataset == "2023Cv123_prompt" || dataset == "2023Cv123_ZB_prompt")
{
	jec = getFJC("",                                                               // Winter23Prompt23_RunC_V2_DATA_L1FastJet_AK4PFPuppi",
			//"Winter23Prompt23_RunC_V2_DATA_L2Relative_AK4PFPuppi",
			"Summer23Run3_V1_MC_L2Relative_AK4PUPPI",                         // Mikel
			// "Run23C123-Prompt_DATA_L2L3Residual_AK4PFPuppi"
			"Summer23Prompt23_Run2023Cv123_V2_DATA_L2L3Residual_AK4PFPuppi"); //"Winter23Prompt23_RunC_V2_DATA_L2L3Residual_AK4PFPuppi");
}

if (dataset == "2023Cv4" || dataset == "2023Cv4_ZB" || 
		dataset == "2023Cv4_prompt" || dataset == "2023Cv4_ZB_prompt")
{
	jec = getFJC("",                                                             // Winter23Prompt23_RunC_V2_DATA_L1FastJet_AK4PFPuppi",
			//"Winter23Prompt23_RunC_V2_DATA_L2Relative_AK4PFPuppi",
			"Summer23Run3_V1_MC_L2Relative_AK4PUPPI",                       // Mikel
			//"Run23C4-Prompt_DATA_L2L3Residual_AK4PFPuppi"
			"Summer23Prompt23_Run2023Cv4_V2_DATA_L2L3Residual_AK4PFPuppi"); //"Winter23Prompt23_RunC_V2_DATA_L2L3Residual_AK4PFPuppi");
}

if (dataset == "2023D" || dataset == "2023D_ZB" ||
		dataset == "2023D_prompt" || dataset == "2023D_ZB_prompt")
{
	jec = getFJC("",                                                           // Winter23Prompt23_RunC_V2_DATA_L1FastJet_AK4PFPuppi",
			//"Winter23Prompt23_RunC_V2_DATA_L2Relative_AK4PFPuppi",
			"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI",                     // Mikel
			//"Run23D-Prompt_DATA_L2L3Residual_AK4PFPuppi"
			//"Summer23Prompt23_Run2023D_V2_DATA_L2L3Residual_AK4PFPuppi"); //"Winter23Prompt23_RunC_V2_DATA_L2L3Residual_AK4PFPuppi");
	    "Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
}

if (TString(dataset.c_str()).Contains("Winter24MCFlat") )
{
	jec = getFJC("",
			"Winter24Run3_V1_MC_L2Relative_AK4PUPPI"
			//"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI", // To compare with Summer23MGBPix
			//"Summer23Run3_V1_MC_L2Relative_AK4PUPPI", // To compare with Summer23MG
			"");
	jerpath = "CondFormats/JetMETObjects/data/Summer22EEVetoRun3_V1_NSCP_MC_PtResolution_ak4puppi.txt"; // Same as Summer22EE, until updated
	//jerpathsf = "CondFormats/JetMETObjects/data/Summer23_2023D_JRV1_MC_SF_AK4PFPuppi.txt"; // To compare with Summer23MGBPix
	jerpathsf = "";
	//jersfvspt = getFJC("", "Summer23_2023D_JRV1_MC_SF_AK4PFPuppi", "");
	//jersfvspt = getFJC("", "Summer23_2023D_JRV2_MC_SF_AK4PFPuppi", "");
	jersfvspt = getFJC("", "", "");;

	useJERSFvsPt = true;
}

if (TString(dataset.c_str()).Contains("Winter24MG"))
{
	jec = getFJC("",
			"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
			"");
	jerpathsf = "";
	//jerpathsf = "CondFormats/JetMETObjects/data/Prompt24_2024F_JRV5M_MC_SF_AK4PFPuppi.txt";
	//jersfvspt = getFJC("", "Prompt24_2024F_JRV5M_MC_SF_AK4PFPuppi", "");
	jersfvspt = getFJC("", "", "");
	jerpath = "CondFormats/JetMETObjects/data/Summer23BPixPrompt23_RunD_JRV1_MC_PtResolution_AK4PFPuppi.txt";
	//jerpath = "";
	useJERSFvsPt = false; //Nestor, Sep20, 2024.

           	
	   if (reweightPU && !doPU_per_trigger)
	   {
	      if (TString(dataset.c_str()).Contains("Winter24MGV14_")) {
	         TFile f("luminosityscripts/PUWeights/PUWeight2024F/PUWeights_HLT_PFJet500_2024F.root");
	         pileupRatio = (TH1D *)f.Get("pileup_weights_HLT_PFJet500_2024F");
	         pileupRatio->SetDirectory(0);
	         // Print mean, min weight, max weight
	         cout << "Pileup ratio mean = " << pileupRatio->GetMean() << endl;
	         cout << "Pileup ratio min = " << pileupRatio->GetMinimum() << endl;
	         cout << "Pileup ratio max = " << pileupRatio->GetMaximum() << endl;

	      } else {
	         TFile f("luminosityscripts/PUWeights/Winter24MG_PUWeight.root");
	         pileupRatio = (TH1D *)f.Get("pileup");
	         pileupRatio->SetDirectory(0);
	         // Print mean, min weight, max weight
	         cout << "Pileup ratio mean = " << pileupRatio->GetMean() << endl;
	         cout << "Pileup ratio min = " << pileupRatio->GetMinimum() << endl;
	         cout << "Pileup ratio max = " << pileupRatio->GetMaximum() << endl;

	      }
	   }
}



if (TString(dataset.c_str()).Contains("2024B")  || dataset == "2024B_ZB")
{
	if (TString(dataset.c_str()).Contains("2024BR"))
	{
		jec = getFJC(""
				"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
				//"Prompt24_Run2024CR_V3M_DATA_L2L3Residual_AK4PFPuppi");
		    "Prompt24_Run2024CR_V4M_DATA_L2L3Residual_AK4PFPuppi");
	}
	else {
		jec = getFJC("",
				"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
				//"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI", // BPix D
				//"Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
				//"Summer23Prompt23_Run2023D_V2_DATA_L2L3Residual_AK4PFPuppi"); // Prompt V2
				//"Prompt24_Run2024BC_V1M_DATA_L2L3Residual_AK4PFPuppi");
				//"Prompt24_Run2024BC_V2M_DATA_L2L3Residual_AK4PFPuppi");
				//"Prompt24_Run2024BCD_V3M_DATA_L2L3Residual_AK4PFPuppi");
				//"Prompt24_Run2024BCD_V4M_DATA_L2L3Residual_AK4PFPuppi");
		    "Prompt24_Run2024BCD_V5M_DATA_L2L3Residual_AK4PFPuppi");
	}
}

if (TString(dataset.c_str()).Contains("2024C")  || dataset == "2024C_ZB")
{ 
	if (TString(dataset.c_str()).Contains("2024CR"))
	{
		jec = getFJC(""
				"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
				//"Prompt24_Run2024CR_V3M_DATA_L2L3Residual_AK4PFPuppi");
		    "Prompt24_Run2024CR_V4M_DATA_L2L3Residual_AK4PFPuppi");
	}
	else if (dataset == "2024CS" || dataset == "2024CT")
	{
		jec = getFJC(""
				"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
				//"Prompt24_Run2024CR_V3M_DATA_L2L3Residual_AK4PFPuppi");
		    "Prompt24_Run2024CS_V4M_DATA_L2L3Residual_AK4PFPuppi");
	}

	else {
		jec = getFJC("",
				"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
				//"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI", // BPix D
				//"Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
				//"Summer23Prompt23_Run2023D_V2_DATA_L2L3Residual_AK4PFPuppi"); // Prompt V2
				//"Prompt24_Run2024BC_V1M_DATA_L2L3Residual_AK4PFPuppi");
				//"Prompt24_Run2024BC_V2M_DATA_L2L3Residual_AK4PFPuppi");
				//"Prompt24_Run2024BCD_V3M_DATA_L2L3Residual_AK4PFPuppi");
				//"Prompt24_Run2024BCD_V4M_DATA_L2L3Residual_AK4PFPuppi");
		    "Prompt24_Run2024BCD_V5M_DATA_L2L3Residual_AK4PFPuppi");
	}
}

if (TString(dataset.c_str()).Contains("2024D")  || dataset == "2024D_ZB")
{
	jec = getFJC("",
			"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
			//"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI", // BPix D
			//"Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
			//"Summer23Prompt23_Run2023D_V2_DATA_L2L3Residual_AK4PFPuppi"); // Prompt V2
			//"Prompt24_Run2024BC_V1M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BC_V2M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BCD_V3M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BCD_V4M_DATA_L2L3Residual_AK4PFPuppi");
	    "Prompt24_Run2024BCD_V5M_DATA_L2L3Residual_AK4PFPuppi");
}

if (TString(dataset.c_str()).Contains("2024E")  || dataset == "2024Ev1_ZB" || dataset == "2024Ev2_ZB" )
{
	jec = getFJC("",
			"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
			//"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI", // BPix D
			//"Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
			//"Summer23Prompt23_Run2023D_V2_DATA_L2L3Residual_AK4PFPuppi"); // Prompt V2
			//"Prompt24_Run2024BC_V1M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BC_V2M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BCD_V3M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024E_V4M_DATA_L2L3Residual_AK4PFPuppi");
	    "Prompt24_Run2024E_V5M_DATA_L2L3Residual_AK4PFPuppi");

}

if (TString(dataset.c_str()).Contains("2024F")  || dataset == "2024F_ZB")
{
	jec = getFJC("",
			"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
			//"Summer23BPixRun3_V3_MC_L2Relative_AK4PUPPI", // BPix D
			//"Summer23BPixPrompt23_RunD_V1_DATA_L2L3Residual_AK4PFPuppi");
			//"Summer23Prompt23_Run2023D_V2_DATA_L2L3Residual_AK4PFPuppi"); // Prompt V2
			//"Prompt24_Run2024BC_V1M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BC_V2M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024BCD_V3M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024E_V4M_DATA_L2L3Residual_AK4PFPuppi");
			//"Prompt24_Run2024CS_V4M_DATA_L2L3Residual_AK4PFPuppi");
	    "Prompt24_Run2024F_V5M_DATA_L2L3Residual_AK4PFPuppi");

}

if (dataset == "2024G"  || dataset == "2024G_ZB")
{
	jec = getFJC("",
			"Winter24Run3_V1_MC_L2Relative_AK4PUPPI",
			"Prompt24_Run2024F_V5M_DATA_L2L3Residual_AK4PFPuppi");

}

if ((isRun2 && (!jec || !jecl1rc)) || (isRun3 && !jec))
	cout << "Missing files for " << dataset << endl
	<< flush;
	assert(jec);
	// assert(jecl1rc);

if (debug)
	cout << "Setting up JER smearing" << endl
	<< flush;

	// Smear JER
	// NB: could implement time dependence as in jetphys/IOV.h
	JME::JetResolution *jer(0);
	JME::JetResolutionScaleFactor *jersf(0);
if (isMC && smearJets)
{
	cout << jerpath << endl
		<< flush;
	if (!useJERSFvsPt)
		cout << jerpathsf << endl
			<< flush;
	if (jerpath == "" || (jerpathsf == "" && !useJERSFvsPt))
		cout << "Missing JER file paths for " << dataset << endl
			<< flush;
	assert(jerpath != "");
	assert(jerpathsf != "" || useJERSFvsPt);
	assert(jersfvspt || !useJERSFvsPt);
	jer = new JME::JetResolution(jerpath.c_str());
	if (!useJERSFvsPt)
		jersf = new JME::JetResolutionScaleFactor(jerpathsf.c_str());
	//cout << "jersf from the SF file" << endl
	//   << flush;
	if (!jer || (!jersf && !useJERSFvsPt) || (!jersfvspt && useJERSFvsPt))
		cout << "Missing JER files for " << dataset << endl
			<< flush;
}

TLorentzVector p4rawmet, p4t1met, p4mht, p4l1rc, p4dj;
// TLorentzVector p4, p4s, p4mht, p4mht2, p4mhtc, p4mhtc3, p4t, p4p;
TLorentzVector p4, /*p4raw,*/ p4g, p4s, p4t, p4p;
TLorentzVector p4lead, p4recoil; //, p4other;
TLorentzVector p4leadRES, p4recoilRES;
TLorentzVector p4b3, p4b3r, p4b3l, p4m;
TLorentzVector p4b, p4bt, p4bp, p4bx, p4d, p4dx;
TLorentzVector p4c, p4cx, p4f, p4fx, p4l, p4r;
TLorentzVector p4m0, p4m2, p4mn, p4mu; //, p4mo;
TLorentzVector p4m3, p4mn3, p4mu3;
TLorentzVector p4corrjets, p4rcjets, p4rawjets;
TFile *fout = new TFile(Form("rootfiles/%s/jmenano_%s_out_%s_%s.root",
			version.c_str(),
			isMC ? "mc" : "data",
			dataset.c_str(), version.c_str()),
		"RECREATE");

// Monitor trigger rates
TH1D *htrg = new TH1D("htrg", "Triggers;Trigger;N_{events}",
		vtrg.size(), 0, vtrg.size());
//cout << "AAAA :"  <<htrg->GetNbinsX() << endl;
for (int i = 1; i != htrg->GetNbinsX() + 1; ++i)
{
	htrg->GetXaxis()->SetBinLabel(i, vtrg[i - 1].c_str());
}
//cout << "BBB :"  <<htrg->GetNbinsX() << endl;

// trigger vs lumi
TH2D *h_trgvslumi = new TH2D("h_trgvslumi", "Triggers;Trigger;Lumi",
		vtrg.size(), 0, vtrg.size(), 100, 0, 1);
for (int i = 1; i != h_trgvslumi->GetNbinsX() + 1; ++i)
{
	h_trgvslumi->GetXaxis()->SetBinLabel(i, vtrg[i - 1].c_str());
}

// trigger vs pu
TH2D *h_trgvspu = new TH2D("h_trgvspu", "Triggers;Trigger;PU",
		vtrg.size(), 0, vtrg.size(), 100, 0, 100);
for (int i = 1; i != h_trgvspu->GetNbinsX() + 1; ++i)
{
	h_trgvspu->GetXaxis()->SetBinLabel(i, vtrg[i - 1].c_str());
}


if (debug)
	cout << "Setting up histograms" << endl
	<< flush;

	// Setup HT bin weighting and monitoring
	TH1D *hxsec(0), *hnevt(0), *hnwgt(0), *hLHE_HT(0), *hLHE_HTw(0), *hHT(0), *hHT_Now(0), *hHT_MCw(0), *hHT_w(0);
	double vht2[] = {0, 25, 50, 100, 200, 300, 500, 700, 1000, 1500, 2000, 13800};
const int nht2 = sizeof(vht2) / sizeof(vht2[0]) - 1;
double vht3[] = {0, 40, 70, 100, 200, 400, 600, 800, 1000, 1200, 1500, 2000,
	13800};
const int nht3 = sizeof(vht3) / sizeof(vht3[0]) - 1;
const double *vht = (isRun3 ? &vht3[0] : &vht2[0]);
const int nht = (isRun3 ? nht3 : nht2);
int nMG(0), sumwMG(0);
if (isMG)
{

	hxsec = new TH1D("hxsec", ";H_{T} (GeV);pb", nht, vht);
	hnevt = new TH1D("hnevt", ";H_{T} (GeV);N_{evt}", nht, vht);
	hnwgt = new TH1D("hnwgt", ";H_{T} (GeV);Sum of weights", nht, vht);
	hLHE_HT = new TH1D("hLHE_HT", ";H_{T} (GeV);N_{evt} (unweighted)", nht, vht);
	hLHE_HTw = new TH1D("hLHE_HTw", ";H_{T} (GeV);N_{evt} (weighted)", nht, vht);
    hHT = new TH1D("hHT", ";H_{T} (GeV);N_{evt} (weighted)", 2490, 10, 2500);
    hHT_Now = new TH1D("hHT_Now", ";H_{T} (GeV);N_{evt} (unweighted)", 2490, 10, 2500);
    hHT_MCw = new TH1D("hHT_MCw", ";H_{T} (GeV);N_{evt} (MC weight event)", 2490, 10, 2500);
    hHT_w = new TH1D("hHT_w", ";genWeight;N_{evt} (genWeight)", 2490, 10, 2500);

    // Reference number of events, retrieved manually with
    // TChain c("Events"); c.AddFile("<path to files>/*.root"); c.GetEntries();
    // Also re-calculated this code before event loop when needed
    int vnevt2[nht2] = {0, 0, 11197186, 23002929, 17512439, 16405924, 14359110,
                        13473185, 4365993, 2944561, 1836165};
    // int vnevt3[nht3] = {0, 9929, 26573, 16411, 10495, 8260, 7929, 10082, 14390, 6548, 6250}; // Summer22MG, local files
    int vnevt3[nht3] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Summer22MG, local files
    int vnwgt3[nht3] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Summer22MG, local files
    const int *vnevt = (isRun3 ? &vnevt3[0] : &vnevt2[0]);
    const int *vnwgt = (isRun3 ? &vnwgt3[0] : &vnevt2[0]);
    for (int i = 0; i != nht; ++i)
    {
      hnevt->SetBinContent(i + 1, vnevt[i]);
      hnwgt->SetBinContent(i + 1, vnwgt[i]);
      nMG += vnevt[i];
      sumwMG += vnwgt[i];
    }
    cout << "Loaded Hefaistos MadGraph event numbers ("
         << nMG << ")" << endl
         << flush;

    // xsec from jetphys/settings.h_template
    double vxsec2[nht2] = {0, 0, 246300000. * 23700000. / 28060000., 23700000,
                           1547000, 322600, 29980, 6334, 1088, 99.11, 20.23};
    // Run3 xsec from Mikel Mendizabal, MatterMost 16 Oct 2023
    // SUMMER22
    //double vxsec3[nht3] =
    //    {0, 3.136e+08,
    //     5.883e+07, // 5.807e+07 * 3478241.4 / 5305581.5, // HT 70to100
    //     2.520e+07, 1.936e+06, 9.728e+04,
    //     1.323e+04, // 3.044e+04, //HT 60to800
    //     3.027e+03, 8.883e+02, 3.834e+02, 1.253e+02, 2.629e+01};

    // SUMMER23, Fikri MatterMost 22.2.2024
    double vxsec3[nht3] =
        {0, 3.131e+08,
         5.892e+07,
         2.532e+07, 1.964e+06, 9.690e+04,
         1.360e+04,
         3.052e+03, 8.929e+02, 3.863e+02, 1.267e+02, 2.665e+01};
    const double *vxsec = (isRun3 ? &vxsec3[0] : &vxsec2[0]);
    for (int i = 0; i != nht; ++i)
    {
      hxsec->SetBinContent(i + 1, vxsec[i]);
    }
  } // isMG

  // Inclusive jets pT binning
  double vpti[] =
      {1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,
       97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,
       507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248,
       1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500,
       2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832, 4037, 4252, 4477, 4713,
       4961, 5220, 5492, 5777, 6076, 6389, 6717, 7000};
  double npti = sizeof(vpti) / sizeof(vpti[0]) - 1;
  // L2Res pT binning (central+forward hybrid)
  double vptd[] =
      //{59.,85.,104.,170.,236., 302., 370., 460., 575.}; // central
      //{86., 110., 132., 204., 279., 373.} // forward
      {10, 15, 21, 28, 37, 49,
       59, 86, 110, 132, 170, 204, 236, 279, 302, 373, 460, 575,
       638, 737, 846, 967, 1101, 1248,
       1410, 1588, 1784, 2000, 2238, 2500, 2787, 3103, 3450, 4037, 5220};
  double nptd = sizeof(vptd) / sizeof(vptd[0]) - 1;
  // L3Res (gamma+jet) pT binning adapted and extended
  const double vpt[] = {10, 15, 20, 25, 30, 35,
                        40, 50, 60, 70, 85, 100, 125, 155, 180, 210, 250, 300,
                        350, 400, 500, 600, 800, 1000, 1200, 1500,
                        1800, 2100, 2400, 2700, 3000, 3500, 4000, 5000};
  const int npt = sizeof(vpt) / sizeof(vpt[0]) - 1;

  // Regular L2Relative eta binning
  double vx[] =
      {-5.191,
       -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489,
       -3.314, -3.139, -2.964, -2.853, -2.65, -2.5, -2.322, -2.172, -2.043,
       -1.93, -1.83, -1.74, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218,
       -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435,
       -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435,
       0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305,
       1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5,
       2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191,
       4.363, 4.538, 4.716, 4.889, 5.191};
  const int nx = sizeof(vx) / sizeof(vx[0]) - 1;
  // Current L2Res |eta| binning from Jindrich
  // https://indico.cern.ch/event/1263476/contributions/5311425/attachments/2612023/4513129/L2Res+HDM-March15.pdf
  double vxd[] =
      //{0, 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191};
      // Newer L2Res |eta| binning from Mikel
      // https://indico.cern.ch/event/1335203/#7-update-on-l2res-for-2022-rer
      //{0., 0.261, 0.522, 0.783, 1.044, 1.305, 1.479, 1.653, 1.93, 2.172, 2.322, 2.5, 2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.839, 4.013, 4.583, 5.191};
      
      // Nestor April 3, 2024: |eta| binning to compare with DESY results
      {0, 0.087, 0.174, 0.261, 0.348, 0.435,
       0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305,
       1.392, 1.479, 1.566, 1.653, 1.74, 1.83, 1.93, 2.043, 2.172, 2.322, 2.5,
       2.65, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191,
       4.363, 4.538, 4.716, 4.889, 5.191};

  const int nxd = sizeof(vxd) / sizeof(vxd[0]) - 1;

  // p_reco/p_gen binning
  int reco_nedges = 101;
  double minValue = 0.0;
  double maxValue = 2.0;
  double stepSize = (maxValue - minValue) / (reco_nedges - 1);
  // Create the vector
  std::vector<double> vres(reco_nedges);

  // Populate the vector with values
  for (int i = 0; i < reco_nedges; ++i) {
      vres[i] = minValue + i * stepSize;
  }
  const int nres = sizeof(vres) / sizeof(vres[0]) - 1;

  // Binning for 3D histo. Nestor. April 25, 2024. Format example (xAxis.size()-1, xAxis.data())
  // phi binning
  int phi_nedges = 73;
  double phi_minValue = -TMath::Pi();
  double phi_maxValue = +TMath::Pi();
  double phi_stepSize = (phi_maxValue - phi_minValue) / (phi_nedges - 1);
  // Create the vector
  std::vector<double> phi_vx(phi_nedges);

  // Populate the vector with values
  for (int i = 0; i < phi_nedges; ++i) {
      phi_vx[i] = phi_minValue + i * phi_stepSize;
  }

  // Asymm binning
  int asymm_nedges = 201;
  double asymm_minValue = -1.0;
  double asymm_maxValue = 1.0;
  double asymm_stepSize = (asymm_maxValue - asymm_minValue) / (asymm_nedges - 1);
  // Create the vector
  std::vector<double> asymm_vx(asymm_nedges);

  // Populate the vector with values
  for (int i = 0; i < asymm_nedges; ++i) {
      asymm_vx[i] = asymm_minValue + i * asymm_stepSize;
  }

  // JER studies
  // Asymm binning
  int MPF_nedges = 201;
  double MPF_minValue = 0.0;
  double MPF_maxValue = 2.0;
  double MPF_stepSize = (MPF_maxValue - MPF_minValue) / (MPF_nedges - 1);
  // Create the vector
  std::vector<double> MPF_vx(MPF_nedges);

  // Populate the vector with values
  for (int i = 0; i < MPF_nedges; ++i) {
      MPF_vx[i] = MPF_minValue + i * MPF_stepSize;
  }
  //

  const int ny = 800;
  double vy[ny + 1];
  for (int i = 0; i != ny + 1; ++i)
    vy[i] = -3. + (+5. + 3.) / ny * i;
  const int nz = 400;
  double vz[nz + 1];
  for (int i = 0; i != nz + 1; ++i)
    vz[i] = -1. + (+3. + 1.) / nz * i;

  // Extras for JMENANO
  TH2D *h2mhtvsmet = new TH2D("h2mhtvsmet", "MHT vs MET;MET;MHT",
                              500, 0, 500, 500, 0, 500);
  TH2D *h2dphi = new TH2D("h2dphi", "#Delta#phi vs #eta;#eta;#Delta#phi",
                          nx, vx, 126, -TMath::TwoPi(), +TMath::TwoPi());

TH1D *h_PUProfile(0), *h_RhoAll(0), *h_NPV(0), *h_NPVGood(0);
if (do_PUProfiles){
   fout->mkdir("Profiles");
   fout->cd("Profiles");
   if (isMC){
      h_PUProfile = new TH1D("h_PUProfile", "PUProfile", 119, 0, 120);
   }
   //h_PUProfile = new TH1D("h_PUProfile", "PUProfile", 119, 0, 120);
   h_RhoAll = new TH1D("h_RhoAll", "RhoFastjetAll", 119, 0, 120);
   h_NPV = new TH1D("h_NPV", "NPV", 119, 0, 120);
   h_NPVGood = new TH1D("h_NPVGood", "NPVGood", 119, 0, 120);
}

  // L2Res profiles for HDM method
  // coding: m0=MPF, m2=DB, mn=n-jet, mu=uncl. (observable)
  //         a=PtAve, t=PtTag, p=PtProbe (binning)
  //         b=bisector, c=central(tag), f=forward(probe) (projection axis)
  fout->mkdir("Refs");
  fout->cd("Refs");
  TH1D *hnjet = new TH1D("hnjet", "hnjet", 500, 0, 500);

  // Nestor, 22 July 2024: Add the option to use the luminosity.
  bool doluminosity = true; //Nestor. xsection plot. April 17, 2024.
  bool dolumi;
  if (doluminosity)
  {  if (isMC)
       dolumi = false;
     else{
       dolumi = true;
       LoadLumi();
     }
  }
  std::cout << "dolumi: " << dolumi << std::endl;
    //LoadLumi();

  /*
  // PF composition plots
  // Copy L2Res histograms for multiple pT bins
  const double vpt[] = {15, 20, 25, 30, 35,
      40, 50, 60, 70, 85, 100, 125, 155, 180, 210, 250, 300,
      350, 400, 500, 600, 800, 1000, 1200, 1500,
      1800, 2100, 2400, 2700, 3000};
  const int npt = sizeof(vpt)/sizeof(vpt[0])-1;

  TH1D *hptbins = new TH1D("hptbins",";p_{T} (GeV);N_{events}",npt,vpt);

  TProfile *pjes = new TProfile("pjes","JES",nx,vx);
  TProfile *pres = new TProfile("pres","RES",nx,vx);
  TProfile *pdjes = new TProfile("pdjes","#DELTAJES",nx,vx);
  TProfile *pjes13 = new TProfile("pjes13","JES",npt,vpt);
  TProfile *pres13 = new TProfile("pres13","RES",npt,vpt);
  TProfile *pdjes13 = new TProfile("pdjes13","#DELTAJES",npt,vpt);

  TProfile *pchf = new TProfile("pchf","CHF",nx,vx);
  TProfile *pnhf = new TProfile("pnhf","NHF",nx,vx);
  TProfile *pnef = new TProfile("pnef","NEF",nx,vx);
  TProfile *pcef = new TProfile("pcef","CEF",nx,vx);
  TProfile *pmuf = new TProfile("pmuf","MUF",nx,vx);
  TProfile *phef = new TProfile("phfhf","HEF",nx,vx);
  TProfile *phhf = new TProfile("phfef","HHF",nx,vx);

  TProfile *pchf13 = new TProfile("pchf13","CHF",npt,vpt);
  TProfile *pnhf13 = new TProfile("pnhf13","NHF",npt,vpt);
  TProfile *pnef13 = new TProfile("pnef13","NEF",npt,vpt);
  TProfile *pcef13 = new TProfile("pcef13","CEF",npt,vpt);
  TProfile *pmuf13 = new TProfile("pmuf13","MUF",npt,vpt);

  TH1D *hmjj2 = new TH1D("hmjj2","Dijet mass, 2-lead",1000,0,1000);
  TH1D *hmjj213 = new TH1D("hmjj213","Dijet mass, |#DeltaEta|<1.3, 2-lead",
           1000,0,1000);
  */
  fout->cd();

  if (debug)
    cout << "Load JSON (or not)" << endl
         << flush;

  bool doJSON = (true && !isMC);
  if (doJSON)
  {
    // if (!LoadJSON()) {
    // cout << "Issues loading the JSON file; aborting..." << endl << flush;
    // exit(0);
    // }

    // Load JSON files
    if (TString(dataset.c_str()).Contains("2016"))
      LoadJSON("rootfiles/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt");
    if (TString(dataset.c_str()).Contains("2017"))
      LoadJSON("rootfiles/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt");
    if (TString(dataset.c_str()).Contains("2018"))
      LoadJSON("rootfiles/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt");
    if (TString(dataset.c_str()).Contains("2022"))
      LoadJSON("rootfiles/Cert_Collisions2022_355100_362760_Golden.json");
    if (TString(dataset.c_str()).Contains("2023"))
      LoadJSON("rootfiles/Cert_Collisions2023_366442_370790_Golden.json");
    if (TString(dataset.c_str()).Contains("2024"))
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379154_DCSOnly_TkPx.json"); // April 10, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379355_DCSOnly_TkPx.json"); // April 14, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379454_DCSOnly_TkPx.json"); // April 15, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379470_DCSOnly_TkPx.json"); // April 16, 2024, 19:30
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379530_DCSOnly_TkPx.json"); // April 17, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379618_DCSOnly_TkPx.json"); // April 18, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_379774_DCSOnly_TkPx.json"); // April 21, 2024, 19:31
      //LoadJSON("rootfiles/Cert_Collisions2024_378981_379075_Golden.json"); // Released April 18, implemented April 22
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380074_DCSOnly_TkPx.json"); // April 29, 2024, 19:31
      //LoadJSON("rootfiles/Cert_Collisions2024_378981_379470_Golden.json"); // April 30, 2024, 09:57
      //LoadJSON("rootfiles/Cert_Collisions2024_378981_379866_Golden.json"); // May 3, 2024, 12:55
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380238_DCSOnly_TkPx.json"); // May 2, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380403_DCSOnly_TkPx.json"); // May 6, 2024, 11:51
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380533_DCSOnly_TkPx.json"); // May 9, 2024, 19:31
      //LoadJSON("rootfiles/Cert_Collisions2024_378981_380115_Golden.json"); // May 13, 2024, 19:33
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380649_DCSOnly_TkPx.json"); // May 14, 2024, 19:31
      //LoadJSON("rootfiles/Cert_Collisions2024_378981_380470_Golden.json"); // May 16, 2024, 11:13
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380649_DCSOnly_TkPx.json"); // May 16, 2024, 19:30
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_380963_DCSOnly_TkPx.json"); // May 21, 2024, 19:31
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_381199_DCSOnly_TkPx.json"); // May 26, 2024, 19:31
      //LoadJSON("rootfiles/Cert_Collisions2024_378981_380649_Golden.json"); // May 27, 2024, 19:31
      //LoadJSON("rootfiles/Cert_Collisions2024_eraB_Golden.json");
      //LoadJSON("rootfiles/Collisions24_13p6TeV_378981_381478_DCSOnly_TkPx.json"); // June 2, 2024.
      //LoadJSON("rootfiles/CombinedJSON_GoldenRuns_378985to380945_DCSRuns_380946to381516.json");
      //LoadJSON("rootfiles/CombinedJSON_GoldenRuns_378985to381152_DCSRuns_381153to381594.json");
      //LoadJSON("rootfiles/CombinedJSONS_GoldenRuns_378985to381417_DCSRuns_381418to381594.json");
      //LoadJSON("rootfiles/CombinedJSON_GoldenRuns_378981to382329_DCSRuns_382330to382686.json");
      //LoadJSON("rootfiles/CombinedJSON_GoldenRuns_378985to383163_DCSRuns_383164to383467.json");
      LoadJSON("rootfiles/CombinedJSON_GoldenRuns_378981to383724_DCSRuns_383725to384446.json");

  }
  int _nbadevts_json(0);

  bool doTrigger = true; //(true && !isMC);
  int _nbadevts_trg(0);
  int _nbadevts_fwdtrg(0);
  int _ngoodevts(0);

  // Listing of runs and LS
  int nrun(0), nls(0), nevt(0);
  map<int, map<int, int>> mrunls;

  map<string, mctruthHistos *> mhmc;
  map<string, jetvetoHistos *> mhjv;
  map<string, incjetHistos *> mhij;
  map<string, dijetHistos *> mhdj;
  map<string, gluonHistos *> mhgj;
  map<string, dijetHistos2 *> mhdj2;
  map<string, multijetHistos *> mhmj;
  map<string, lumiHistos *> mhlumi;
  map<string, jetsperRuns *> mjet;

  //bool dolumi = true; //Nestor. xsection plot. April 17, 2024.
  //if (dolumi)
  //  LoadLumi();

  for (int itrg = 0; itrg != ntrg; ++itrg)
  {

    if (debug)
      cout << "Trigger " << vtrg[itrg] << endl
           << flush;

    fout->mkdir(vtrg[itrg].c_str());
    fout->cd(vtrg[itrg].c_str());
    TDirectory *dout = gDirectory;

    // Figure out trigger pT threshold from the name
    int trgpt(-1), nfound(0);
    if (nfound != 1)
    {
      nfound = (vtrg[itrg] == "HLT_ZeroBias" || vtrg[itrg] == "HLT_MC" ? 1 : 0);
      trgpt = 0;
    }
    if (nfound != 1)
      nfound = sscanf(vtrg[itrg].c_str(), "HLT_PFJet%d", &trgpt);
    if (nfound != 1)
      nfound = sscanf(vtrg[itrg].c_str(), "HLT_PFJetFwd%d", &trgpt);
    if (nfound != 1)
      nfound = sscanf(vtrg[itrg].c_str(), "HLT_DiPFJetAve%d", &trgpt);
    if (nfound != 1)
      nfound = sscanf(vtrg[itrg].c_str(), "HLT_DiPFJetAve%d_HFJEC", &trgpt);
    if (!(nfound == 1 && trgpt != -1))
    {
      cout << "trigger " << vtrg[itrg] << ": nfound=" << nfound
           << ", trgpt = " << trgpt << endl
           << flush;
    }
    assert(nfound == 1);
    assert(trgpt != -1);

    if (isMC && doMCtruth && vtrg[itrg] == "HLT_MC")
    {

      if (debug)
        cout << "Setup MC truth" << endl
             << flush;

      dout->mkdir("MCtruth");
      dout->cd("MCtruth");

      mctruthHistos *h = new mctruthHistos();

      string &t = vtrg[itrg];
      mhmc[t] = h;
      // h->trg = t;
      // h->trgpt = trgpt;

      // struct range &r  = mt[t];
      // h->ptmin = r.ptmin;
      // h->ptmax = r.ptmax;
      // h->absetamin = r.absetamin;
      // h->absetamax = r.absetamax;

      h->ptreco_ptgen = new TH1D("ptreco/ptgen",";p_{T,reco}/p_{T,gen} (GeV);N_{events};"
		                       "N_{events}",
		            //100, 0.0, 2.0);
                            vres.size()-1, vres.data());
      //h->h1res_bar = new TH1D("res_barrel", ";p_{T,reco}/p_{T,gen} (GeV);N_{events}", 100, 0.0, 2.0);

      h->h2res_ptgen = new TH2D("response_ptgen","p_{T,gen} (GeV);p_{T,gen} (GeV);"
		                       "p_{T,reco}/p_{T,gen} (GeV)",
		            nptd, vptd, 100, 0.0, 2.0);
      // Jet_btagUParTAK4QvG
      h->h2_btagUpar = new TH2D("h2_btagUpar","Jet_btagUParTAK4QvG;p_{T,gen} (GeV);"
                                       "Fraction",
                            nptd, vptd, 100, 0.0, 1.0);
      //
      h->h2res_etagen = new TH2D("response_etagen","|#eta_{gen}|;|#eta_{gen}|;"
		                       "p_{T,reco}/p_{T,gen} (GeV)",
		            nxd, vxd, 100, 0.0, 2.0);
      h->h3res = new TH3D("Response3D", ";#eta_{gen};p_{T,gen} (GeV);p_{T,reco}/p_{T,gen}", 
                        nxd, vxd, nptd, vptd, vres.size()-1, vres.data());
      h->h3res_Match = new TH3D("Response3D Match", ";#eta_{gen};p_{T,gen} (GeV);p_{T,reco}/p_{T,gen}",
                        nxd, vxd, nptd, vptd, vres.size()-1, vres.data());
      h->h3res_raw = new TH3D("Response3D_raw", ";#eta_{gen};p_{T,gen} (GeV);p_{T,reco}/p_{T,gen}",
                        nxd, vxd, nptd, vptd, vres.size()-1, vres.data());
      //h->h2res_bar = new TH2D("response_barrel", ";p_{T,gen} (GeV);p_{T,reco}/p_{T,gen} (GeV);", 
	//	            nptd, vptd, 100, 0.0, 2.0);

      h->h2pteta = new TH2D("h2pteta", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                       "N_{events}",
                            nxd, vxd, nptd, vptd);
      h->h2pteta_gen = new TH2D("h2pteta_gen", ";|#eta_{gen}|;p_{T,gen} (GeV);"
                                               "N_{events}",
                                nxd, vxd, nptd, vptd);
      h->h2pteta_rec = new TH2D("h2pteta_rec", ";|#eta_{jet}|;p_{T,jet} (GeV);"
                                               "N_{events}",
                                nxd, vxd, nptd, vptd);
      h->p2jes = new TProfile2D("p2jes", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                         "JES(jet)",
                                nxd, vxd, nptd, vptd);
      h->p2jsf = new TProfile2D("p2jsf", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                         "JERSF(jet)",
                                nxd, vxd, nptd, vptd);
      h->p2r = new TProfile2D("p2r", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                     "p_{T,jet}/p_{T,gen}",
                              nxd, vxd, nptd, vptd);
      h->p2r_NoMatch = new TProfile2D("p2r_NoMatch", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                     "p_{T,jet}/p_{T,gen}",
                              nxd, vxd, nptd, vptd);
      h->p2r_raw = new TProfile2D("p2r_raw", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                     "p_{T,jet}/p_{T,gen}",
                              nxd, vxd, nptd, vptd);
      h->p2effz = new TProfile2D("p2effz", ";|#eta_{gen}|;p_{T,gen} (GeV);"
                                           "Vertex efficiency",
                                 nxd, vxd, nptd, vptd);
      h->p2eff = new TProfile2D("p2eff", ";|#eta_{gen}|;p_{T,gen} (GeV);"
                                         "Efficiency",
                                nxd, vxd, nptd, vptd);
      h->p2pur = new TProfile2D("p2pur", ";|#eta_{jet}|;p_{T,jet} (GeV);"
                                         "Purity",
                                nxd, vxd, nptd, vptd);

      /*
      if (reweightPU || doPU_per_trigger){
         h->h1_PURho = new TH1D("h1_PURW", ";p_{T,jet} (GeV);" "#rho",
                                        npt, vpt);
	 h->h1_PUMu = new TH1D("h1_PUMu", "#mu", 119, 0, 120);

	 h->h1_PUNPV = new TH1D("h1_PUNPV", "NPV", 119, 0, 120);

	 h->h1_PUNPVGood = new TH1D("h1_PUNPVGood", "NPV_good", 119, 0, 120);
      }
      */

    } // isMC && doMCtruth

    // Jet veto per trigger
    if (doJetveto)
    {
      if (debug)
        cout << "Setup doJetveto " << trgpt << endl
             << flush;

      dout->mkdir("Jetveto");
      dout->cd("Jetveto");

      jetvetoHistos *h = new jetvetoHistos();

      string &t = vtrg[itrg];
      mhjv[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;

      // Plots with inclusive jet selection
      h->h2pteta_all = new TH2D("h2pteta_all", ";#eta;p_{T} (GeV);N_{jet}",
                                nx, vx, npti, vpti);
      h->h2pteta_sel = new TH2D("h2pteta_sel", ";#eta;p_{T} (GeV);N_{jet}",
                                nx, vx, npti, vpti);
      h->h2phieta = new TH2D("h2phieta", ";#eta;#phi;N_{jet}",
                             nx, vx, 72, -TMath::Pi(), +TMath::Pi());

      if (doPFComposition)
      {
        h->p2chf = new TProfile2D("p2chf", ";#eta;#phi;CHF (DM)",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
        h->p2nef = new TProfile2D("p2nef", ";#eta;#phi;NEF (DM)",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
        h->p2nhf = new TProfile2D("p2nhf", ";#eta;#phi;NHF (DM)",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
      }

      // Plots with dijet selection, pTave bins
      if (doJetvetoVariants)
      {
        h->h2ptaeta_all = new TH2D("h2ptaeta_all", ";#eta;p_{T} (GeV);"
                                                   "N_{jet}",
                                   nx, vx, npti, vpti);
        h->h2ptaeta_sel = new TH2D("h2ptaeta_sel", ";#eta;p_{T} (GeV);"
                                                   "N_{jet}",
                                   nx, vx, npti, vpti);
      }
      h->h2phieta_ave = new TH2D("h2phieta_ave", ";#eta;#phi;N_{jet}",
                                 nx, vx, 72, -TMath::Pi(), +TMath::Pi());
      h->p2asymm = new TProfile2D("p2asymm", ";#eta;#phi;Asymmetry",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
      h->p2asymm_noveto = new TProfile2D("p2asymm_noveto", ";#eta;#phi;Asymmetry_noveto",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
      h->p2asymm_noveto_pttag = new TProfile2D("p2asymm_noveto_pttag", ";#eta;#phi;Asymmetry_noveto_pttag",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
      h->h3asymm = new TH3D("h3asymm", ";#eta;#phi;h3asymm",
		        //nx, vx, 72, -TMath::Pi(), +TMath::Pi(), 200, -1, 1);
			nx, vx, phi_vx.size()-1, phi_vx.data(),asymm_vx.size()-1, asymm_vx.data());


      if (doJetvetoVariants)
      {
        // Plots with dijet selection, pTtag bins
        h->h2pteta_all_pttag = new TH2D("h2pteta_all_pttag", ";#eta;p_{T} (GeV);N_{jet}",
                                   nx, vx, npti, vpti);
        h->h2pteta_sel_pttag = new TH2D("h2pteta_sel_pttag", ";#eta;p_{T} (GeV);N_{jet}",
                                   nx, vx, npti, vpti);
        h->h2phieta_pttag = new TH2D("h2phieta_pttag", ";#eta;#phi;N_{jet}",
                                   nx, vx, 72, -TMath::Pi(), +TMath::Pi());

        if (doPFComposition)
        {

          h->p2asymm_pttag = new TProfile2D("p2asymm_pttag", ";#eta;#phi;Asymmetry_pttag",
                                  nx, vx, 72, -TMath::Pi(), +TMath::Pi());
          h->p2chf_pttag = new TProfile2D("p2chf_pttag", ";#eta;#phi;CHF (TP)",
                                      nx, vx, 72, -TMath::Pi(), +TMath::Pi());
          h->p2nef_pttag = new TProfile2D("p2nef_pttag", ";#eta;#phi;NEF (TP)",
                                      nx, vx, 72, -TMath::Pi(), +TMath::Pi());
          h->p2nhf_pttag = new TProfile2D("p2nhf_pttag", ";#eta;#phi;NHF (TP)",
                                      nx, vx, 72, -TMath::Pi(), +TMath::Pi());
          h->p2chf_noveto_pttag = new TProfile2D("p2chf_noveto_pttag", ";#eta;#phi;CHF (TP)",
                                      nx, vx, 72, -TMath::Pi(), +TMath::Pi());
          h->p2nef_noveto_pttag = new TProfile2D("p2nef_noveto_pttag", ";#eta;#phi;NEF (TP)",
                                      nx, vx, 72, -TMath::Pi(), +TMath::Pi());
          h->p2nhf_noveto_pttag = new TProfile2D("p2nhf_noveto_pttag", ";#eta;#phi;NHF (TP)",
                                      nx, vx, 72, -TMath::Pi(), +TMath::Pi());
        }
      }
    } // doJetVeto

    // Inclusive jet per trigger
    if (doIncjet)
    {
      if (debug)
        cout << "Setup doIncjet " << trgpt << endl
             << flush;

      dout->mkdir("Incjet");
      dout->cd("Incjet");

      incjetHistos *h = new incjetHistos();

      string &t = vtrg[itrg];
      mhij[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;

      h->h2pteta_all = new TH2D("h2pteta_all", ";#eta;p_{T} (GeV);N_{jet}",
                                nx, vx, npti, vpti);
      h->h2pteta_sel = new TH2D("h2pteta_sel", ";#eta;p_{T} (GeV);N_{jet}",
                                nx, vx, npti, vpti);
      h->h2pteta = new TH2D("h2pteta", ";#eta;p_{T} (GeV);N_{jet}",
                            nx, vx, npti, vpti);
      h->h2pteta_lumi = new TH2D("h2pteta_lumi", ";#eta;p_{T} (GeV);N_{jet}",
                            nx, vx, npti, vpti);

      // For pileup cleaning checks
      if (isMG)
      {
        h->h2jtvht = new TH2D("h2jtvht", ";H_{T} (GeV);2*Jet[0]",
                              npti, vpti, npti, vpti);
        h->h2jtoht = new TH2D("h2jtoht", ";H_{T} (GeV);2*Jet[0]/HT",
                              npti, vpti, 200, 0., 10.);
      }

      h->hpt13 = new TH1D("hpt13", ";p_{T,jet} (GeV)", npti, vpti);

      h->hpteta20 = new TH1D("hpteta20", ";#eta;Number of entries", nx, vx);

      h->hpteta30 = new TH1D("hpteta30", ";#eta;Number of entries", nx, vx);

      h->hpteta40 = new TH1D("hpteta40", ";#eta;Number of entries", nx, vx);

      h->hpteta50 = new TH1D("hpteta50", ";#eta;Number of entries", nx, vx);


      for (int iy = 0; iy != h->ny; ++iy)
      {
        h->vpt[iy] = new TH1D(Form("hpt%02d", 5 * (iy + 1)), ";p_{T} (GeV);"
                                                             "N_{jet}",
                              npti, vpti);
      } // for iy

      if (doPFComposition)
      {

        dout->mkdir("Incjet/PFcomposition");
        dout->cd("Incjet/PFcomposition");

        h->p2pt = new TProfile2D("p2pt", ";#eta;p_{T,jet} (GeV);"
                                         "p_{T,jet}",
                                 nx, vx, npt, vpt);
        h->p2rho_PURW = new TProfile2D("p2rho_PURW", ";#eta;p_{T,jet} (GeV);"
                                           "#rho",
                                  nx, vx, npt, vpt);
        h->p2rho = new TProfile2D("p2rho", ";#eta;p_{T,jet} (GeV);"
                                           "#rho",
                                  nx, vx, npt, vpt);
        h->p2chf = new TProfile2D("p2chf", ";#eta;p_{T,jet} (GeV);"
                                           "CHF",
                                  nx, vx, npt, vpt);
        h->p2nhf = new TProfile2D("p2nhf", ";#eta;p_{T,jet} (GeV);"
                                           "NHF",
                                  nx, vx, npt, vpt);
        h->p2nef = new TProfile2D("p2nef", ";#eta;p_{T,jet} (GeV);"
                                           "NEF",
                                  nx, vx, npt, vpt);
        h->p2cef = new TProfile2D("p2cef", ";#eta;p_{T,jet} (GeV);"
                                           "CEF",
                                  nx, vx, npt, vpt);
        h->p2muf = new TProfile2D("p2muf", ";#eta;p_{T,jet} (GeV);"
                                           "MUF",
                                  nx, vx, npt, vpt);

        h->ppt13 = new TProfile("ppt13", ";#eta;p_{T,jet} (GeV);"
                                         "p_{T,jet}",
                                npt, vpt);
        h->prho13 = new TProfile("prho13", ";#eta;p_{T,jet} (GeV);"
                                           "#rho",
                                 npt, vpt);
        h->pchf13 = new TProfile("pchf13", ";#eta;p_{T,jet} (GeV);"
                                           "CHF",
                                 npt, vpt);
        h->h2chf13 = new TH2D("h2chf13", ";p_{T,jet} (GeV);CHF;"
                                           "CHF",
                                 npt, vpt, 132, 0, 1+4./128);
        h->pnhf13 = new TProfile("pnhf13", ";#eta;p_{T,jet} (GeV);"
                                           "NHF",
                                 npt, vpt);
        h->pnef13 = new TProfile("pnef13", ";#eta;p_{T,jet} (GeV);"
                                           "NEF",
                                 npt, vpt);
        h->pcef13 = new TProfile("pcef13", ";#eta;p_{T,jet} (GeV);"
                                           "CEF",
                                 npt, vpt);
        h->pmuf13 = new TProfile("pmuf13", ";#eta;p_{T,jet} (GeV);"
                                           "MUF",
                                 npt, vpt);
      }
    } // incjet

    // Dijet per trigger
    //LoadLumi()
    if (doDijet)
    {
      if (debug)
        cout << "Setup doDijet " << trgpt << endl
             << flush;

      dout->mkdir("Dijet");
      dout->cd("Dijet");

      dijetHistos *h = new dijetHistos();

      string &t = vtrg[itrg];
      mhdj[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;

      // Counting of events, and JEC L2L3Res for undoing
      h->h2pteta_aball = new TH2D("h2pteta_aball", ";#eta;p_{T,avp} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->h2pteta_absel = new TH2D("h2pteta_absel", ";#eta;p_{T,avp} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->p2resab = new TProfile2D("p2resab", ";#eta;p_{T,avp} (GeV);"
                                             "JES(probe)/JES(tag)",
                                  nx, vx, npt, vpt);

      // MPF decomposition for HDM method
      h->p2m0ab = new TProfile2D("p2m0ab", ";#eta;p_{T,avp} (GeV);MPF0",
                                 nx, vx, npt, vpt);
      h->p2m2ab = new TProfile2D("p2m2ab", ";#eta;p_{T,avp} (GeV);MPF2",
                                 nx, vx, npt, vpt);
      h->p2mnab = new TProfile2D("p2mnab", ";#eta;p_{T,avp} (GeV);MPFn",
                                 nx, vx, npt, vpt);
      h->p2muab = new TProfile2D("p2muab", ";#eta;p_{T,avp} (GeV);MPFu",
                                 nx, vx, npt, vpt);

      // Variants with different binnings and with error on the mean
      h->h2pteta_adall = new TH2D("h2pteta_adall", ";#eta;p_{T,ave} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->h2pteta_adsel = new TH2D("h2pteta_adsel", ";#eta;p_{T,ave} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->p2resad = new TProfile2D("p2resad", ";#eta;p_{T,ave} (GeV);"
                                             "JES(probe)/JES(tag)",
                                  nx, vx, npt, vpt);

      // MPF decomposition for HDM method
      h->p2m0ad = new TProfile2D("p2m0ad", ";#eta;p_{T,ave} (GeV);MPF0",
                                 nx, vx, npt, vpt);
      h->p2m2ad = new TProfile2D("p2m2ad", ";#eta;p_{T,ave} (GeV);MPF2",
                                 nx, vx, npt, vpt);
      h->p2mnad = new TProfile2D("p2mnad", ";#eta;p_{T,ave} (GeV);MPFn",
                                 nx, vx, npt, vpt);
      h->p2muad = new TProfile2D("p2muad", ";#eta;p_{T,ave} (GeV);MPFu",
                                 nx, vx, npt, vpt);

      h->h2pteta_tcall = new TH2D("h2pteta_tcall", ";#eta;p_{T,ave} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->h2pteta_tcsel = new TH2D("h2pteta_tcsel", ";#eta;p_{T,ave} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->p2restc = new TProfile2D("p2restc", ";#eta;p_{T,ave} (GeV);"
                                             "JES(probe)/JES(tag)",
                                  nx, vx, npt, vpt);

      h->p2m0tc = new TProfile2D("p2m0tc", ";#eta;p_{T,tag} (GeV);MPF0",
                                 nx, vx, npt, vpt);
      h->p2m2tc = new TProfile2D("p2m2tc", ";#eta;p_{T,tag} (GeV);MPF2",
                                 nx, vx, npt, vpt);
      h->p2mntc = new TProfile2D("p2mntc", ";#eta;p_{T,tag} (GeV);MPFn",
                                 nx, vx, npt, vpt);
      h->p2mutc = new TProfile2D("p2mutc", ";#eta;p_{T,tag} (GeV);MPFu",
                                 nx, vx, npt, vpt);

      h->h2pteta_pfall = new TH2D("h2pteta_pfall", ";#eta;p_{T,ave} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->h2pteta_pfsel = new TH2D("h2pteta_pfsel", ";#eta;p_{T,ave} (GeV);"
                                                   "N_{events}",
                                  nx, vx, npt, vpt);
      h->p2respf = new TProfile2D("p2respf", ";#eta;p_{T,ave} (GeV);"
                                             "JES(probe)/JES(tag)",
                                  nx, vx, npt, vpt);

      h->p2m0pf = new TProfile2D("p2m0pf", ";#eta;p_{T,probe} (GeV);MPF0",
                                 nx, vx, npt, vpt);
      h->p2m2pf = new TProfile2D("p2m2pf", ";#eta;p_{T,probe} (GeV);MPF2",
                                 nx, vx, npt, vpt);
      h->p2mnpf = new TProfile2D("p2mnpf", ";#eta;p_{T,probe} (GeV);MPFn",
                                 nx, vx, npt, vpt);
      h->p2mupf = new TProfile2D("p2mupf", ";#eta;p_{T,probe} (GeV);MPFu",
                                 nx, vx, npt, vpt);

      if (doDijetJER)
      {
        dout->mkdir("Dijet/JER");
        dout->cd("Dijet/JER");

        // Basic profiles with RMS as error ("S") for JER studies
        h->p2m0 = new TProfile2D("p2m0", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0 (MPF)",
                                 nx, vx, npt, vpt, "S");
        h->p2m0x = new TProfile2D("p2m0x", ";#eta;p_{T,avp} (GeV);"
                                           "MPFX0 (MPFX)",
                                  nx, vx, npt, vpt, "S");
        h->p2m2 = new TProfile2D("p2m2", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2 (DB)",
                                 nx, vx, npt, vpt, "S");
        h->p2m2x = new TProfile2D("p2m2x", ";#eta;p_{T,avp} (GeV);"
                                           "MPF2 (DBX)",
                                  nx, vx, npt, vpt, "S");
      }

      if (doPFComposition)
      {

        dout->mkdir("Dijet/PFcomposition");
        dout->cd("Dijet/PFcomposition");

        h->p2pt = new TProfile2D("p2pt", ";#eta;p_{T,avp} (GeV);"
                                         "p_{T,probe}",
                                 nx, vx, npt, vpt);
        h->p2rho = new TProfile2D("p2rho", ";#eta;p_{T,avp} (GeV);"
                                           "#rho",
                                  nx, vx, npt, vpt);
        h->p2chf = new TProfile2D("p2chf", ";#eta;p_{T,avp} (GeV);"
                                           "CHF",
                                  nx, vx, npt, vpt);
        h->p2nhf = new TProfile2D("p2nhf", ";#eta;p_{T,avp} (GeV);"
                                           "NHF",
                                  nx, vx, npt, vpt);
        h->p2nef = new TProfile2D("p2nef", ";#eta;p_{T,avp} (GeV);"
                                           "NEF",
                                  nx, vx, npt, vpt);
        h->p2cef = new TProfile2D("p2cef", ";#eta;p_{T,avp} (GeV);"
                                           "CEF",
                                  nx, vx, npt, vpt);
        h->p2muf = new TProfile2D("p2muf", ";#eta;p_{T,avp} (GeV);"
                                           "MUF",
                                  nx, vx, npt, vpt);

        h->ppt13 = new TProfile("ppt13", ";#eta;p_{T,avp} (GeV);"
                                         "p_{T,tag}",
                                npt, vpt);
        h->prho13 = new TProfile("prho13", ";#eta;p_{T,avp} (GeV);"
                                           "#rho",
                                 npt, vpt);
        h->pchf13 = new TProfile("pchf13", ";#eta;p_{T,avp} (GeV);"
                                           "CHF",
                                 npt, vpt);
        h->pnhf13 = new TProfile("pnhf13", ";#eta;p_{T,avp} (GeV);"
                                           "NHF",
                                 npt, vpt);
        h->pnef13 = new TProfile("pnef13", ";#eta;p_{T,avp} (GeV);"
                                           "NEF",
                                 npt, vpt);
        h->pcef13 = new TProfile("pcef13", ";#eta;p_{T,avp} (GeV);"
                                           "CEF",
                                 npt, vpt);
        h->pmuf13 = new TProfile("pmuf13", ";#eta;p_{T,avp} (GeV);"
                                           "MUF",
                                 npt, vpt);
      }

    } // doDijet

    // GluonJets per trigger
    if (doGluonJets)
    {
      if (debug)
        cout << "Setup doGluonJets " << trgpt << endl
             << flush;

      dout->mkdir("GluonJets");
      dout->cd("GluonJets");

      gluonHistos *h = new gluonHistos();

      string &t = vtrg[itrg];
      mhgj[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;


      if (bool doWP_t = true){
        dout->mkdir("GluonJets/tight");
        dout->cd("GluonJets/tight");
      
        // MPF decomposition for HDM method
        h->p2m0ab_t = new TProfile2D("p2m0ab_t", ";#eta;p_{T,avp} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2ab_t = new TProfile2D("p2m2ab_t", ";#eta;p_{T,avp} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnab_t = new TProfile2D("p2mnab_t", ";#eta;p_{T,avp} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2muab_t = new TProfile2D("p2muab_t", ";#eta;p_{T,avp} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        // MPF decomposition for HDM method
        h->p2m0ad_t = new TProfile2D("p2m0ad_t", ";#eta;p_{T,ave} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2ad_t = new TProfile2D("p2m2ad_t", ";#eta;p_{T,ave} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnad_t = new TProfile2D("p2mnad_t", ";#eta;p_{T,ave} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2muad_t = new TProfile2D("p2muad_t", ";#eta;p_{T,ave} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        h->p2m0tc_t = new TProfile2D("p2m0tc_t", ";#eta;p_{T,tag} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2tc_t = new TProfile2D("p2m2tc_t", ";#eta;p_{T,tag} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mntc_t = new TProfile2D("p2mntc_t", ";#eta;p_{T,tag} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2mutc_t = new TProfile2D("p2mutc_t", ";#eta;p_{T,tag} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        h->p2m0pf_t = new TProfile2D("p2m0pf_t", ";#eta;p_{T,probe} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2pf_t = new TProfile2D("p2m2pf_t", ";#eta;p_{T,probe} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnpf_t = new TProfile2D("p2mnpf_t", ";#eta;p_{T,probe} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2mupf_t = new TProfile2D("p2mupf_t", ";#eta;p_{T,probe} (GeV);MPFu",
                                   nx, vx, npt, vpt);
      }
      /*
      if (doDijetJER)
      {
        dout->mkdir("GluonJets/JER");
        dout->cd("GluonJets/JER");

        // Basic profiles with RMS as error ("S") for JER studies
        h->p2m0_t = new TProfile2D("p2m0_t", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0 (MPF)",
                                 nx, vx, npt, vpt, "S");
        h->p2m0x_t = new TProfile2D("p2m0x_t", ";#eta;p_{T,avp} (GeV);"
                                           "MPFX0 (MPFX)",
                                  nx, vx, npt, vpt, "S");
        h->p2m2_t = new TProfile2D("p2m2_t", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2 (DB)",
                                 nx, vx, npt, vpt, "S");
        h->p2m2x_t = new TProfile2D("p2m2x_t", ";#eta;p_{T,avp} (GeV);"
                                           "MPF2 (DBX)",
                                  nx, vx, npt, vpt, "S");
	
      }
      */
      if (bool doWP_m = true){
        dout->mkdir("GluonJets/medium");
        dout->cd("GluonJets/medium");
        // MPF decomposition for HDM method
        h->p2m0ab_m = new TProfile2D("p2m0ab_m", ";#eta;p_{T,avp} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2ab_m = new TProfile2D("p2m2ab_m", ";#eta;p_{T,avp} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnab_m = new TProfile2D("p2mnab_m", ";#eta;p_{T,avp} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2muab_m = new TProfile2D("p2muab_m", ";#eta;p_{T,avp} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        // MPF decomposition for HDM method
        h->p2m0ad_m = new TProfile2D("p2m0ad_m", ";#eta;p_{T,ave} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2ad_m = new TProfile2D("p2m2ad_m", ";#eta;p_{T,ave} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnad_m = new TProfile2D("p2mnad_m", ";#eta;p_{T,ave} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2muad_m = new TProfile2D("p2muad_m", ";#eta;p_{T,ave} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        h->p2m0tc_m = new TProfile2D("p2m0tc_m", ";#eta;p_{T,tag} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2tc_m = new TProfile2D("p2m2tc_m", ";#eta;p_{T,tag} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mntc_m = new TProfile2D("p2mntc_m", ";#eta;p_{T,tag} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2mutc_m = new TProfile2D("p2mutc_m", ";#eta;p_{T,tag} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        h->p2m0pf_m = new TProfile2D("p2m0pf_m", ";#eta;p_{T,probe} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2pf_m = new TProfile2D("p2m2pf_m", ";#eta;p_{T,probe} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnpf_m = new TProfile2D("p2mnpf_m", ";#eta;p_{T,probe} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2mupf_m = new TProfile2D("p2mupf_m", ";#eta;p_{T,probe} (GeV);MPFu",
                                   nx, vx, npt, vpt);
      }
      /*
      if (doDijetJER)
      {
        //dout->mkdir("GluonJets/JER");
        dout->cd("GluonJets/JER");

        // Basic profiles with RMS as error ("S") for JER studies
        h->p2m0_m = new TProfile2D("p2m0_m", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0 (MPF)",
                                 nx, vx, npt, vpt, "S");
        h->p2m0x_m = new TProfile2D("p2m0x_m", ";#eta;p_{T,avp} (GeV);"
                                           "MPFX0 (MPFX)",
                                  nx, vx, npt, vpt, "S");
        h->p2m2_m = new TProfile2D("p2m2_m", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2 (DB)",
                                 nx, vx, npt, vpt, "S");
        h->p2m2x_m = new TProfile2D("p2m2x_m", ";#eta;p_{T,avp} (GeV);"
                                           "MPF2 (DBX)",
                                  nx, vx, npt, vpt, "S");

      }
      */
      if (bool doWP_l = true){
        dout->mkdir("GluonJets/loose");
        dout->cd("GluonJets/loose");
        // MPF decomposition for HDM method
        h->p2m0ab_l = new TProfile2D("p2m0ab_l", ";#eta;p_{T,avp} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2ab_l = new TProfile2D("p2m2ab_l", ";#eta;p_{T,avp} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnab_l = new TProfile2D("p2mnab_l", ";#eta;p_{T,avp} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2muab_l = new TProfile2D("p2muab_l", ";#eta;p_{T,avp} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        // MPF decomposition for HDM method
        h->p2m0ad_l = new TProfile2D("p2m0ad_l", ";#eta;p_{T,ave} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2ad_l = new TProfile2D("p2m2ad_l", ";#eta;p_{T,ave} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnad_l = new TProfile2D("p2mnad_l", ";#eta;p_{T,ave} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2muad_l = new TProfile2D("p2muad_l", ";#eta;p_{T,ave} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        h->p2m0tc_l = new TProfile2D("p2m0tc_l", ";#eta;p_{T,tag} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2tc_l = new TProfile2D("p2m2tc_l", ";#eta;p_{T,tag} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mntc_l = new TProfile2D("p2mntc_l", ";#eta;p_{T,tag} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2mutc_l = new TProfile2D("p2mutc_l", ";#eta;p_{T,tag} (GeV);MPFu",
                                   nx, vx, npt, vpt);


        h->p2m0pf_l = new TProfile2D("p2m0pf_l", ";#eta;p_{T,probe} (GeV);MPF0",
                                   nx, vx, npt, vpt);
        h->p2m2pf_l = new TProfile2D("p2m2pf_l", ";#eta;p_{T,probe} (GeV);MPF2",
                                   nx, vx, npt, vpt);
        h->p2mnpf_l = new TProfile2D("p2mnpf_l", ";#eta;p_{T,probe} (GeV);MPFn",
                                   nx, vx, npt, vpt);
        h->p2mupf_l = new TProfile2D("p2mupf_l", ";#eta;p_{T,probe} (GeV);MPFu",
                                   nx, vx, npt, vpt);
      }

      if (doDijetJER)
      {
        dout->mkdir("GluonJets/JER");
        dout->cd("GluonJets/JER");

        // Basic profiles with RMS as error ("S") for JER studies
        h->p2m0_t = new TProfile2D("p2m0_t", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0 (MPF)",
                                 nx, vx, npt, vpt, "S");
        h->p2m0x_t = new TProfile2D("p2m0x_t", ";#eta;p_{T,avp} (GeV);"
                                           "MPFX0 (MPFX)",
                                  nx, vx, npt, vpt, "S");
        h->p2m2_t = new TProfile2D("p2m2_t", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2 (DB)",
                                 nx, vx, npt, vpt, "S");
        h->p2m2x_t = new TProfile2D("p2m2x_t", ";#eta;p_{T,avp} (GeV);"
                                           "MPF2 (DBX)",
                                  nx, vx, npt, vpt, "S");

        // Basic profiles with RMS as error ("S") for JER studies
        h->p2m0_m = new TProfile2D("p2m0_m", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0 (MPF)",
                                 nx, vx, npt, vpt, "S");
        h->p2m0x_m = new TProfile2D("p2m0x_m", ";#eta;p_{T,avp} (GeV);"
                                           "MPFX0 (MPFX)",
                                  nx, vx, npt, vpt, "S");
        h->p2m2_m = new TProfile2D("p2m2_m", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2 (DB)",
                                 nx, vx, npt, vpt, "S");
        h->p2m2x_m = new TProfile2D("p2m2x_m", ";#eta;p_{T,avp} (GeV);"
                                           "MPF2 (DBX)",
                                  nx, vx, npt, vpt, "S");

        // Basic profiles with RMS as error ("S") for JER studies
        h->p2m0_l = new TProfile2D("p2m0_l", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0 (MPF)",
                                 nx, vx, npt, vpt, "S");
        h->p2m0x_l = new TProfile2D("p2m0x_l", ";#eta;p_{T,avp} (GeV);"
                                           "MPFX0 (MPFX)",
                                  nx, vx, npt, vpt, "S");
        h->p2m2_l = new TProfile2D("p2m2_l", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2 (DB)",
                                 nx, vx, npt, vpt, "S");
        h->p2m2x_l = new TProfile2D("p2m2x_l", ";#eta;p_{T,avp} (GeV);"
                                           "MPF2 (DBX)",
                                  nx, vx, npt, vpt, "S");

      }

    } // GluonJets



    if (doDijet2)
    {
      if (debug)
        cout << "Setup doDijet2 " << trgpt << endl
             << flush;

      dout->mkdir("Dijet2");
      dout->cd("Dijet2");

      dijetHistos2 *h = new dijetHistos2();

      string &t = vtrg[itrg];
      mhdj2[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;

      // Counting of events, and JEC L2L3Res+JERSF for undoing
      h->h2pteta = new TH2D("h2pteta", ";#eta;p_{T,avp} (GeV);"
                                       "N_{events}",
                            nxd, vxd, nptd, vptd);
      h->p2res = new TProfile2D("p2res", ";#eta;p_{T,avp} (GeV);"
                                         "JES(probe)/JES(tag)",
                                nxd, vxd, nptd, vptd);
      h->p2jsf = new TProfile2D("p2jsf", ";#eta;p_{T,avp} (GeV);"
                                         "JERSF(probe)/JERSF(tag)",
                                nxd, vxd, nptd, vptd);

      // MPF decomposition for HDM method
      h->p2m0 = new TProfile2D("p2m0", ";#eta;p_{T,avp} (GeV);MPF0",
                               nxd, vxd, nptd, vptd);
      h->p2m2 = new TProfile2D("p2m2", ";#eta;p_{T,avp} (GeV);MPF2",
                               nxd, vxd, nptd, vptd);
      h->p2mn = new TProfile2D("p2mn", ";#eta;p_{T,avp} (GeV);MPFn",
                               nxd, vxd, nptd, vptd);
      h->p2mu = new TProfile2D("p2mu", ";#eta;p_{T,avp} (GeV);MPFu",
                               nxd, vxd, nptd, vptd);

      h->p2m0x = new TProfile2D("p2m0x", ";#eta;p_{T,avp} (GeV);"
                                         "MPF0X (MPFX)",
                                nxd, vxd, nptd, vptd, "S");
      h->p2m2x = new TProfile2D("p2m2x", ";#eta;p_{T,avp} (GeV);"
                                         "MPF2X (DBX)",
                                nxd, vxd, nptd, vptd, "S");

      // For JER studies
      h->h3m0 = new TH3D("h3m0", ";#eta;p_{T,avp} (GeV);MPF0",
                         nxd, vxd, nptd, vptd, MPF_vx.size()-1,MPF_vx.data());
      h->h3m2 = new TH3D("h3m2", ";#eta;p_{T,avp} (GeV);MPF2",
                         nxd, vxd, nptd, vptd, MPF_vx.size()-1, MPF_vx.data());
      
      // Extra for FRS studies
      h->p2mnu = new TProfile2D("p2mnu", ";#eta;p_{T,avp} (GeV);MPFnu",
                                nxd, vxd, nptd, vptd);
      h->p2mnx = new TProfile2D("p2mnx", ";#eta;p_{T,avp} (GeV);"
                                         "MPFNX",
                                nxd, vxd, nptd, vptd, "S");
      h->p2mux = new TProfile2D("p2mux", ";#eta;p_{T,avp} (GeV);"
                                         "MPFUX",
                                nxd, vxd, nptd, vptd, "S");
      h->p2mnux = new TProfile2D("p2mnux", ";#eta;p_{T,avp} (GeV);"
                                           "MPFNUX",
                                 nxd, vxd, nptd, vptd, "S");

      h->h2ptetatc = new TH2D("h2ptetatc", ";#eta;p_{T,tag} (GeV);"
                                           "N_{events}",
                              nxd, vxd, nptd, vptd);
      h->p2restc = new TProfile2D("p2restc", ";#eta;p_{T,tag} (GeV);"
                                             "JES(probe)/JES(tag)",
                                  nxd, vxd, nptd, vptd);
      h->p2jsftc = new TProfile2D("p2jsftc ", ";#eta;p_{T,tag} (GeV);"
                                              "JERSF(probe)/JERSF(tag)",
                                  nxd, vxd, nptd, vptd);
      h->p2m0tc = new TProfile2D("p2m0tc", ";#eta;p_{T,tag} (GeV);MPF0",
                                 nxd, vxd, nptd, vptd);
      h->p2m2tc = new TProfile2D("p2m2tc", ";#eta;p_{T,tag} (GeV);MPF2",
                                 nxd, vxd, nptd, vptd);
      h->p2mntc = new TProfile2D("p2mntc", ";#eta;p_{T,tag} (GeV);MPFn",
                                 nxd, vxd, nptd, vptd);
      h->p2mutc = new TProfile2D("p2mutc", ";#eta;p_{T,tag} (GeV);MPFu",
                                 nxd, vxd, nptd, vptd);

      h->h2ptetapf = new TH2D("h2ptetapf", ";#eta;p_{T,probe} (GeV);"
                                           "N_{events}",
                              nxd, vxd, nptd, vptd);
      h->p2respf = new TProfile2D("p2respf", ";#eta;p_{T,probe} (GeV);"
                                             "JES(probe)/JES(tag)",
                                  nxd, vxd, nptd, vptd);
      h->p2jsfpf = new TProfile2D("p2jsfpf", ";#eta;p_{T,probe} (GeV);"
                                             "JERSF(probe)/JERSF(tag)",
                                  nxd, vxd, nptd, vptd);
      h->p2m0pf = new TProfile2D("p2m0pf", ";#eta;p_{T,probe} (GeV);MPF0",
                                 nxd, vxd, nptd, vptd);
      h->p2m2pf = new TProfile2D("p2m2pf", ";#eta;p_{T,probe} (GeV);MPF2",
                                 nxd, vxd, nptd, vptd);
      h->p2mnpf = new TProfile2D("p2mnpf", ";#eta;p_{T,probe} (GeV);MPFn",
                                 nxd, vxd, nptd, vptd);
      h->p2mupf = new TProfile2D("p2mupf", ";#eta;p_{T,probe} (GeV);MPFu",
                                 nxd, vxd, nptd, vptd);

      if (doDijet2NM)
      {
        assert(false);
        // separate TProfile2D for NM=1, NM=2, NM=3+ (no JetID if has NM>1)
      }
    } // doDijet2

    // Multijet per trigger
    if (doMultijet)
    {

      if (debug)
        cout << "Setup doMultijet " << trgpt << endl
             << flush;

      dout->mkdir("Multijet");
      dout->cd("Multijet");

      multijetHistos *h = new multijetHistos();

      string &t = vtrg[itrg];
      mhmj[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;

      h->hpta_all = new TH1D("hpta_all", "", npti, vpti);
      h->hpta_sel = new TH1D("hpta_sel", "", npti, vpti);
      h->presa = new TProfile("presa", "", npti, vpti);
      h->ptleada = new TProfile("ptleada", "", npti, vpti);
      h->pcrecoila = new TProfile("pcrecoila", "", npti, vpti);

      h->pm0a = new TProfile("pm0a", "", npti, vpti);
      h->pm2a = new TProfile("pm2a", "", npti, vpti);
      h->pmna = new TProfile("pmna", "", npti, vpti);
      h->pmua = new TProfile("pmua", "", npti, vpti);

      h->hptm_all = new TH1D("hptm_all", "", npti, vpti);
      h->hptm_sel = new TH1D("hptm_sel", "", npti, vpti);
      h->presm = new TProfile("presm", "", npti, vpti);
      h->ptleadm = new TProfile("ptleadm", "", npti, vpti);
      h->pcrecoilm = new TProfile("pcrecoilm", "", npti, vpti);

      h->pm0m = new TProfile("pm0m", "", npti, vpti);
      h->pm2m = new TProfile("pm2m", "", npti, vpti);
      h->pmnm = new TProfile("pmnm", "", npti, vpti);
      h->pmum = new TProfile("pmum", "", npti, vpti);

      h->hptl_all = new TH1D("hptl_all", "", npti, vpti);
      h->hptl_sel = new TH1D("hptl_sel", "", npti, vpti);
      h->presl = new TProfile("presl", "", npti, vpti);
      h->ptleadl = new TProfile("ptleadl", "", npti, vpti);
      h->pcrecoill = new TProfile("pcrecoill", "", npti, vpti);

      h->pm0l = new TProfile("pm0l", "", npti, vpti);
      h->pm2l = new TProfile("pm2l", "", npti, vpti);
      h->pmnl = new TProfile("pmnl", "", npti, vpti);
      h->pmul = new TProfile("pmul", "", npti, vpti);

      h->hptr_all = new TH1D("hptr_all", "", npti, vpti);
      h->hptr_sel = new TH1D("hptr_sel", "", npti, vpti);
      h->presr = new TProfile("presr", "", npti, vpti);
      h->ptleadr = new TProfile("ptleadr", "", npti, vpti);
      h->pcrecoilr = new TProfile("pcrecoilr", "", npti, vpti);

      h->pm0r = new TProfile("pm0r", "", npti, vpti);
      h->pm2r = new TProfile("pm2r", "", npti, vpti);
      h->pmnr = new TProfile("pmnr", "", npti, vpti);
      h->pmur = new TProfile("pmur", "", npti, vpti);

      if (doMultijetControl)
      {
        h->h2m0a = new TH2D("h2m0a", "", npti, vpti, 200, -1, 3);
        h->h2m2a = new TH2D("h2m2a", "", npti, vpti, 200, -1, 3);
        h->hcosdphi = new TH1D("hcosdphi", "", 102, -1.01, 1.01);
      }
      if (doMultijet2Drecoil)
      {
        dout->mkdir("Multijet/2Drecoil");
        dout->cd("Multijet/2Drecoil");
        h->h2recoila = new TH2D("h2recoila", "", npti, vpti, npti, vpti);
        h->h2recoilm = new TH2D("h2recoilm", "", npti, vpti, npti, vpti);
        h->h2recoill = new TH2D("h2recoill", "", npti, vpti, npti, vpti);
        h->h2recoilr = new TH2D("h2recoilr", "", npti, vpti, npti, vpti);
      }
      if (doPFComposition)
      {
        dout->mkdir("Multijet/PFcomposition");
        dout->cd("Multijet/PFcomposition");

        h->ppt13 = new TProfile("ppt13", ";#eta;p_{T,avp} (GeV);"
                                         "p_{T,lead}",
                                npti, vpti);
        h->prho13 = new TProfile("prho13", ";#eta;p_{T,avp} (GeV);"
                                           "#rho",
                                 npti, vpti);
        h->pchf13 = new TProfile("pchf13", ";#eta;p_{T,avp} (GeV);"
                                           "CHF",
                                 npti, vpti);
        h->pnhf13 = new TProfile("pnhf13", ";#eta;p_{T,avp} (GeV);"
                                           "NHF",
                                 npti, vpti);
        h->pnef13 = new TProfile("pnef13", ";#eta;p_{T,avp} (GeV);"
                                           "NEF",
                                 npti, vpti);
        h->pcef13 = new TProfile("pcef13", ";#eta;p_{T,avp} (GeV);"
                                           "CEF",
                                 npti, vpti);
        h->pmuf13 = new TProfile("pmuf13", ";#eta;p_{T,avp} (GeV);"
                                           "MUF",
                                 npti, vpti);

        h->ppt25 = new TProfile("ppt25", ";#eta;p_{T,avp} (GeV);"
                                         "p_{T,recoil}",
                                npti, vpti);
        h->prho25 = new TProfile("prho25", ";#eta;p_{T,avp} (GeV);"
                                           "#rho",
                                 npti, vpti);
        h->pchf25 = new TProfile("pchf25", ";#eta;p_{T,avp} (GeV);"
                                           "CHF",
                                 npti, vpti);
        h->pnhf25 = new TProfile("pnhf25", ";#eta;p_{T,avp} (GeV);"
                                           "NHF",
                                 npti, vpti);
        h->pnef25 = new TProfile("pnef25", ";#eta;p_{T,avp} (GeV);"
                                           "NEF",
                                 npti, vpti);
        h->pcef25 = new TProfile("pcef25", ";#eta;p_{T,avp} (GeV);"
                                           "CEF",
                                 npti, vpti);
        h->pmuf25 = new TProfile("pmuf25", ";#eta;p_{T,avp} (GeV);"
                                           "MUF",
                                 npti, vpti);
      }

    } // doMultijet


    
    // Lumi per trigger
    /* 
    if (dolumi)
    {
      dout->mkdir("LumiInfo");
      dout->cd("LumiInfo");

      lumiHistos *h = new lumiHistos();

      string &t = vtrg[itrg];
      mhlumi[t] = h;

      h->trg = t;
      h->trgpt = trgpt;

      h->lum = 0.0;
      h->lum2 = 0.0;

      h->lumsum = 0.0;
      h->lumsum2 = 0.0;
      h->htrglumi = new TH1D("htrglumi", "", 100, 0, 1);
      h->htrgpu = new TH1D("htrgpu", "", 100, 0, 100);
      //h->hLumiPerRun = new TH1F("hLumiPerRun", "Average Luminosity per Run", 1, 0, 0);
      //h->hnpv = new TH1D("hnpv", "", 100, 0, 100);
      h->hnpvgood = new TH1D("hnpvgood", "", 100, 0, 100);
    }
    */

    
    // Jets per runs per triggers
    if (doJetsperRuns && dolumi)
    {

      if (debug)
        cout << "Setup doJetsperRuns " << trgpt << endl
             << flush;

      dout->mkdir("JetsperRuns");
      dout->cd("JetsperRuns");

      jetsperRuns *h = new jetsperRuns();

      string &t = vtrg[itrg];
      mjet[t] = h;
      h->trg = t;
      h->trgpt = trgpt;

      struct range &r = mt[t];
      h->ptmin = r.ptmin;
      h->ptmax = r.ptmax;
      h->absetamin = r.absetamin;
      h->absetamax = r.absetamax;
      //for (int i = 0; i < _runNumberBin.size(); ++i) {
      //  std::cout << i << " " << _runNumberBin[i] << std::endl;
      //}
      //std::cout << std::endl;
      //std::cout << "_runNumberBin size: "  << _runNumberBin.size() <<  std::endl;
      
      //std::cout << "binEdges: ";
      //for (auto it4 = _runNumberBin.begin(); it4 != _runNumberBin.end(); ++it4) {
       // std::cout << *it4;
        //if (it4 != _runNumberBin.end() - 1) { // Print a comma after all elements except the last one
         //   std::cout << ", ";
       // }
      //}
      h->h1pt13 = new TH1D("h1pt13", ";p_{T,jet} (GeV);Jets;", npti, vpti);
      h->h1pt13_w = new TH1D("h1pt13_w", ";p_{T,jet} (GeV);xsec;", npti, vpti); 
      h->h1jetrate = new TH1D("h1jetrate", ";RunNumber;Rate;", _runNumberBin.size()-1, _runNumberBin.data());
      h->h1jetxsec = new TH1D("h1jetxsec", ";RunNumber;xsec;", _runNumberBin.size()-1, _runNumberBin.data());
      h->h2jetpteta = new TH2D("h2jetpteta", ";|#eta_{jet}|;p_{T,gen} (GeV);"
                                          "N_{events}",
                               nxd, vxd, nptd, vptd);
      /*
      h->p2MPF = new TProfile2D("p2MPF", ";RunNumber;p_{T,lead} (GeV);"
                                         "MPF",
                                 _runNumberBin.size()-1, _runNumberBin.data(), npti, vpti);
      h->p2MPF_bar = new TProfile2D("p2MPF_bar", ";RunNumber;p_{T,lead} (GeV);"
                                         "MPF",
                                 _runNumberBin.size()-1, _runNumberBin.data(), npti, vpti);
      */
      if (itrg == 9) // itrg 9 represents a specific trigger
      { 
        h->pMPF_500 = new TProfile("pMPF_500", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_500b = new TProfile("pMPF_500b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_600 = new TProfile("pMPF_600", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_600b = new TProfile("pMPF_600b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_800 = new TProfile("pMPF_800", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_800b = new TProfile("pMPF_800b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_1000 = new TProfile("pMPF_1000", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_1000b = new TProfile("pMPF_1000b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_1200 = new TProfile("pMPF_1200", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pMPF_1200b = new TProfile("pMPF_1200b", "", _runNumberBin.size()-1, _runNumberBin.data());

        h->pDB_500 = new TProfile("pDB_500", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_500b = new TProfile("pDB_500b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_600 = new TProfile("pDB_600", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_600b = new TProfile("pDB_600b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_800 = new TProfile("pDB_800", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_800b = new TProfile("pDB_800b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_1000 = new TProfile("pDB_1000", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_1000b = new TProfile("pDB_1000b", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_1200 = new TProfile("pDB_1200", "", _runNumberBin.size()-1, _runNumberBin.data());
        h->pDB_1200b = new TProfile("pDB_1200b", "", _runNumberBin.size()-1, _runNumberBin.data());
      }

    } // doJetsperRuns
    

  } // for itrg


  if (debugevent)
    cout << "Load jet veto maps" << endl
         << flush;

  // Load veto maps
  // JECDatabase/jet_veto_maps/Summer19UL16_V0/hotjets-UL16.root
  // JECDatabase/jet_veto_maps/Summer19UL17_V2/hotjets-UL17_v2.root
  // JECDatabase/jet_veto_maps/Summer19UL18_V1/hotjets-UL18.root
  TFile *fjv(0);
  if (isRun2 == 1 || isRun2 == 2) // TString(ds.c_str()).Contains("2016"))
    fjv = new TFile("rootfiles/hotjets-UL16.root", "READ");
  if (isRun2 == 3) // TString(ds.c_str()).Contains("2017"))
    fjv = new TFile("rootfiles/hotjets-UL17_v2.root", "READ");
  if (isRun2 == 4 || TString(dataset.c_str()).Contains("2018"))
    fjv = new TFile("rootfiles/hotjets-UL18.root", "READ");
  if (dataset == "2022C" || dataset == "2022D" || dataset == "2022C_ZB" ||
      dataset == "2022D_ZB" || dataset == "Summer22" ||
      dataset == "Summer22Flat" ||
      TString(dataset.c_str()).Contains("Summer22MG") ||
      TString(dataset.c_str()).Contains("Summer22MC") ||
      dataset == "2022C_prompt" || dataset == "2022C_ZB_prompt" || dataset == "2022D_prompt" || dataset == "2022D_ZB_prompt")
    fjv = new TFile("rootfiles/jetveto2022CD.root", "READ");
  if (dataset == "2022E" || dataset == "2022F" || dataset == "2022G" ||
      dataset == "2022F1" || dataset == "2022F2" ||
      dataset == "2022E_ZB" || dataset == "2022F_ZB" || dataset == "2022G_ZB" ||
      dataset == "Summer22EE" ||
      dataset == "Summer22EEFlat" ||
      TString(dataset.c_str()).Contains("Summer22EEMG"))
    fjv = new TFile("rootfiles/jetveto2022EFG.root", "READ");
  if (dataset == "2023B" || dataset == "2023C" || dataset == "2023BCv123" ||
      dataset == "2023Cv123" || dataset == "2023Cv4" ||
      dataset == "2023B_ZB" || dataset == "2023C_ZB" || dataset == "2023BCv123_ZB" ||
      dataset == "2023Cv123_ZB" || dataset == "2023Cv4_ZB" ||
      dataset == "Summer23MCFlat" || dataset == "Summer23MCBPixFlat" ||
      dataset == "2023Cv123_prompt" || dataset == "2023Cv123_ZB_prompt" || 
      dataset == "2023Cv4_prompt" || dataset == "2023Cv4_ZB_prompt" ||
      TString(dataset.c_str()).Contains("Summer23MC") ||
      (TString(dataset.c_str()).Contains("Summer23MG") && ! TString(dataset.c_str()).Contains("MGBPix")))
    fjv = new TFile("rootfiles/jetveto2023BC.root", "READ");
  if (dataset == "2023D" || dataset == "2023D_ZB" ||
      dataset == "2023D_prompt" || dataset == "2023D_ZB_prompt" ||
      TString(dataset.c_str()).Contains("Summer23MGBPix") || TString(dataset.c_str()).Contains("Summer23MCBPix"))
    fjv = new TFile("rootfiles/jetveto2023D.root", "READ");
  if (dataset == "2024B" || dataset == "2024B_ZB" || dataset == "2024C" || dataset == "2024C_ZB" ||
      dataset == "2024D" || dataset == "2024D_ZB" || dataset == "2024Ev1" || dataset == "2024Ev1_ZB" ||
      dataset == "2024Ev2" || dataset == "2024Ev2_ZB" || dataset == "2024BR" || dataset == "2024CR" ||
      dataset == "2024CS" || dataset == "2024CT" ||
      TString(dataset.c_str()).Contains("Winter24MCFlat") || TString(dataset.c_str()).Contains("Winter24MG"))
    //fjv = new TFile("rootfiles/jetveto2024BC_V1M.root", "READ");
    //fjv = new TFile("rootfiles/jetveto2024BC_V2M.root", "READ");
    //fjv = new TFile("rootfiles/jetveto2024BCD_V3M.root", "READ");
    fjv = new TFile("rootfiles/jetveto2024BCDE.root", "READ");
  if (TString(dataset.c_str()).Contains("2024F") || dataset == "2024F_ZB" || dataset == "2024G" || dataset == "2024G_ZB")
    fjv = new TFile("rootfiles/jetveto2024F.root", "READ");
  assert(fjv);

  // Veto lists for different years (NB: extra MC map for UL16):
  // h2hot_ul16_plus_hbm2_hbp12_qie11 + h2hot_mc (for UL16)
  // h2hot_ul17_plus_hep17_plus_hbpw89 (UL17)
  // h2hot_ul18_plus_hem1516_and_hbp2m1 (UL18)
  TH2D *h2jv(0);
  // To add BPix veto for prompt 2024
  TH2D *h2jvBPix(0);
  if (isRun2 == 1 || isRun2 == 2)
  { // TString(ds.c_str()).Contains("2016")) {
    h2jv = (TH2D *)fjv->Get("h2hot_ul16_plus_hbm2_hbp12_qie11");
    assert(h2jv);
    TH2D *h2mc = (TH2D *)fjv->Get("h2hot_mc");
    assert(h2mc);
    h2jv->Add(h2mc);
  }
  if (isRun2 == 3) // TString(ds.c_str()).Contains("2017"))
    h2jv = (TH2D *)fjv->Get("h2hot_ul17_plus_hep17_plus_hbpw89");
  if (isRun2 == 4 || TString(dataset.c_str()).Contains("2018"))
    h2jv = (TH2D *)fjv->Get("h2hot_ul18_plus_hem1516_and_hbp2m1");
  if (dataset == "2022C" || dataset == "2022D" || dataset == "2022C_ZB" ||
      dataset == "2022D_ZB" || dataset == "Summer22" ||
      dataset == "Summer22Flat" ||
      TString(dataset.c_str()).Contains("Summer22MG") ||
      TString(dataset.c_str()).Contains("Summer22MC") ||
      dataset == "2022C_prompt" || dataset == "2022C_ZB_prompt" || dataset == "2022D_prompt" || dataset == "2022D_ZB_prompt")
    h2jv = (TH2D *)fjv->Get("jetvetomap");
  if (dataset == "2022E" || dataset == "2022F" || dataset == "2022G" ||
      dataset == "2022F1" || dataset == "2022F2" ||
      dataset == "2022E_ZB" || dataset == "2022F_ZB" || dataset == "2022G_ZB" ||
      dataset == "Summer22EE" ||
      dataset == "Summer22EEFlat" ||
      TString(dataset.c_str()).Contains("Summer22EEMG"))
    h2jv = (TH2D *)fjv->Get("jetvetomap");
  if (dataset == "2023B" || dataset == "2023C" || dataset == "2023BCv123" ||
      dataset == "2023Cv123" || dataset == "2023Cv4" ||
      dataset == "2023B_ZB" || dataset == "2023C_ZB" || dataset == "2023BCv123_ZB" ||
      dataset == "2023Cv123_ZB" || dataset == "2023Cv4_ZB" ||
      dataset == "2023Cv123_prompt" || dataset == "2023Cv123_ZB_prompt" ||
      dataset == "2023Cv4_prompt" || dataset == "2023Cv4_ZB_prompt" ||
      dataset == "Summer23MCFlat" || dataset == "Summer23MCBPixFlat" ||
      TString(dataset.c_str()).Contains("Summer23MC") ||
      (TString(dataset.c_str()).Contains("Summer23MG") && ! TString(dataset.c_str()).Contains("MGBPix")))
    h2jv = (TH2D *)fjv->Get("jetvetomap");
  if (dataset == "2023D" || dataset == "2023D_ZB" ||
      dataset == "2023D_prompt" || dataset == "2023D_ZB_prompt" ||
      TString(dataset.c_str()).Contains("Summer23MGBPix") || TString(dataset.c_str()).Contains("Summer23MCBPix"))
    h2jv = (TH2D *)fjv->Get("jetvetomap");
  //if (TString(dataset.c_str()).Contains("2024")  || TString(dataset.c_str()).Contains("Winter24MCFlat") || TString(dataset.c_str()).Contains("Winter24MG"))
    //h2jv = (TH2D *)fjv->Get("jetvetomap");
    //h2jvBPix = (TH2D *)fjv->Get("jetvetomap_bpix");
  if (dataset == "2024B" || dataset == "2024B_ZB" || dataset == "2024C" || dataset == "2024C_ZB" ||
      dataset == "2024D" || dataset == "2024D_ZB" || dataset == "2024Ev1" || dataset == "2024Ev1_ZB" ||
      dataset == "2024Ev2" || dataset == "2024Ev2_ZB" || dataset == "2024BR" || dataset == "2024CR" ||
      dataset == "2024CS" || dataset == "2024CT" ||
      TString(dataset.c_str()).Contains("Winter24MCFlat") || TString(dataset.c_str()).Contains("Winter24MG"))
    h2jv = (TH2D *)fjv->Get("jetvetomap_all");
  if (TString(dataset.c_str()).Contains("2024F") || dataset == "2024F_ZB" || dataset == "2024G" || dataset == "2024G_ZB")
    h2jv = (TH2D *)fjv->Get("jetvetomap_all");
  assert(h2jv);


  // Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nentries = fChain->GetEntries(); // Long startup time
  cout << "Loaded " << nentries << " entries" << endl
       << flush;

  if (isMG && nentries != nMG)
  {
    cout << "Nentries = " << nentries << ", expected nMG = " << nMG << endl
         << flush;
    // assert(false);
    cout << "Recalculate HT bin counts prior to starting."
         << " This will take a few minutes" << endl;
    hnevt->Reset();
    hnwgt->Reset();
    for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0)
        break;
      b_LHE_HT->GetEntry(ientry);    // read only this branch
      b_genWeight->GetEntry(ientry); // also this if MG
      double w = genWeight;
      double w_ptlead = genWeight;
      double w_ptavp3 = genWeight;
      double w_ptave = genWeight;
      double w_ptrecoil = genWeight;
      hnevt->Fill(LHE_HT);
      hnwgt->Fill(LHE_HT, w);
      if (jentry % 1000000 == 0)
        cout << "." << flush;
      if (jentry % 50000000 == 0 && jentry != 0)
        cout << "\nn=" << jentry << endl
             << flush;
    } // for jentry
    nMG = nentries;
    cout << "\nProcessed " << nMG << " entries" << endl
         << flush;
    cout << Form("int vnevt[%d] = ", hnevt->GetNbinsX());
    for (int i = 1; i != hnevt->GetNbinsX() + 1; ++i)
    {
      cout << Form("%s%d", (i == 1 ? "{" : ", "), int(hnevt->GetBinContent(i) + 0.5));
    }
    cout << "}; // " << dataset << endl
         << flush;
    cout << Form("double vnwgt[%d] = ", hnwgt->GetNbinsX());
    for (int i = 1; i != hnwgt->GetNbinsX() + 1; ++i)
    {
      cout << Form("%s%1.4g", (i == 1 ? "{" : ", "), hnwgt->GetBinContent(i));
    }
    cout << "}; // " << dataset << endl
         << flush;
  } // isMC && nentries!=nMG

  // For trigger matching studies
  // const int kMaxTrigJet = 3;
  // Float_t Jet_hltPt[kMaxTrigJet];
  // Float_t Jet_hltPtClose[kMaxTrigJet];
  // Float_t Jet_hltPtNear[kMaxTrigJet];
  // Float_t Jet_hltPtMax[kMaxTrigJet];
  Float_t Jet_RES[nJetMax];
  Float_t Jet_deltaJES[nJetMax];
  Float_t Jet_CF[nJetMax];
  Float_t Jet_genDR[nJetMax];
  // Float_t Jet_smearFactor[nJetMax];

  /*
  //To check the luminosity and the function Load Lumi is called 
  bool is_load_lumi = LoadLumi(); // try to load lumi information
  cout << "Load Lumi test: " << is_load_lumi << endl;
  cout << "_lumsum test: " << _lumsum << endl;
  */ 

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++)
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    if (jentry % 100000 == 0)
      cout << "." << flush;
    if (jentry % 5000000 == 0)
      cout << "n=" << jentry << endl
           << flush;

    if (jentry == 100000 || jentry == 1000000 || jentry == 1000000 ||
        (jentry % 1000000 == 0 && jentry < 10000000) ||
        (jentry % 10000000 == 0) || jentry == nentries - 1)
    {
      if (jentry == 0)
      {
        laptime.Start();
      }
      if (nentries != 0)
      {
        double events_per_second = (double)jentry / fulltime.RealTime(); // Calculate events per second
        cout << Form("\nProcessed %lld events (%1.1f%%) in %1.0f sec. "
                     "(%1.0f sec. for last %d)",
                     jentry, 100. * jentry / nentries, fulltime.RealTime(),
                     laptime.RealTime(), nlap);
	cout << Form("\nEvents per second: %1.2f", events_per_second); // Print events per second
      }
      //cout << "Flag befeore the jets entry " << endl;
      if (jentry != 0 && nlap != 0)
      {
        cout << Form("\nEstimated runtime:  %1.0f sec. "
                     " (%1.0f sec. for last %d)\n",
                     1. * nentries / jentry * fulltime.RealTime(),
                     1. * nentries / nlap * laptime.RealTime(), nlap)
             << flush;
        // Nestor
        //
        TDatime estimated_completion_time;
        estimated_completion_time.Set(start_time.Convert() + (1. * nentries / jentry * fulltime.RealTime()));
        // Format and print the estimated completion time
        cout << "Estimated full runtime: " << estimated_completion_time.AsSQLString() << endl;
        //
        laptime.Reset();
        nlap = 0;
      }
      if (jentry == 0)
        fulltime.Reset(); // Leave out initialization time
      fulltime.Continue();
      laptime.Continue();
    }
    if (jentry % 10000 == 0)
      cout << "." << flush;
    ++nlap;

    if (debugevent)
      cout << "Read run+LS branches for JSON " << endl
           << flush;

    // Clean code from bad lumisections using JSON file
    if (doJSON)
    {
      if (debugevent)
        cout << "doJSON: Read in branches" << endl
             << flush;

      b_run->GetEntry(ientry);
      b_luminosityBlock->GetEntry(ientry);
 
      
      // Does the run/LS pass the latest JSON selection?
      if (_json[run][luminosityBlock] == 0)
      {
        ++_nbadevts_json;
	//cout << "run: " << run << endl;
	//cout << "luminosityBlock: " << luminosityBlock << endl;
	//cout << "nbadevts: " << _nbadevts_json << endl;
        continue;
      }
      

      /*
      if (_json.find(run) != _json.end())
      {
          if (_json[run].find(luminosityBlock) != _json[run].end())
          {
              if (_json[run][luminosityBlock] == 0)
              {
                  ++_nbadevts_json;
                  cout << "run: " << run << endl;
                  cout << "luminosityBlock: " << luminosityBlock << endl;
                  cout << "nbadevts: " << _nbadevts_json << endl;
                  continue;
              }
          }
          else
          {
             // Handle the case where the luminosityBlock does not exist for this run
             cout << "luminosityBlock " << luminosityBlock << " does not exist for run " << run << "." << endl;
          }
      }
      else
      {
         // Handle the case where the run does not exist in the map
         cout << "run " << run << " does not exist in the map." << endl;
      }
      */

      // Separate 2023Cv123 and 2023Cv4
      if (((dataset == "2023Cv123") && (run >= 367765) ) || ((dataset == "2023Cv123") && (run >= 367765))) {
        continue;
      }
      if ((dataset == "2023Cv4") && (run < 367765)) {
        continue;
      }
    } // doJSON

    

    if (debugevent)
      cout << "Read in entry" << endl
           << flush;

    // Read rest of the event before trigger decision
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    double w = (isMC ? genWeight : 1.);
    double w_ptlead = (isMC ? genWeight : 1.); // = w;
    double w_ptavp3 = (isMC ? genWeight : 1.);; // = w;
    double w_ptave = (isMC ? genWeight : 1.);; // = w;
    double w_ptrecoil = (isMC ? genWeight : 1.); // = w;
    //double w = (isMC ? genWeight : (genWeight/lumsum));
    if (isMG)
    {

      int iht = hxsec->FindBin(LHE_HT);
      // Check that iht is equal to HT_bin_idx
      if (iht != HT_bin_idx)
      {
        cout << "LHE_HT = " << LHE_HT << " is not in the bin " << HT_bin_idx << " for " << _filename << endl << flush;
        cout << "Using bin based on filename instead." << endl << flush;
        iht = HT_bin_idx;
      }

      double xsec = hxsec->GetBinContent(iht);
      double nevt = (isRun3 ? hnwgt->GetBinContent(iht) : hnevt->GetBinContent(iht));
      double wht = (nevt ? xsec / nevt : 1);
      w *= wht;
      w_ptlead *= wht;
      w_ptavp3 *= wht;
      w_ptave *= wht;
      w_ptrecoil *= wht;
      if (w > 10000 || w_ptlead > 10000 || w_ptavp3 > 10000 || w_ptrecoil > 10000 ){
        cout << "WARNING: w = " << w << " for " << _filename << endl << flush;
      }
      hLHE_HT->Fill(LHE_HT);     // cross-check hnevt afterwards
      hLHE_HTw->Fill(LHE_HT, w); // cross-check hnwgt afterwards
      hHT->Fill(LHE_HT, w);      // cross-check HT spectrum smoothness afterwards
      hHT_Now->Fill(LHE_HT); 
      hHT_MCw->Fill(LHE_HT, genWeight); 
      hHT_w->Fill(genWeight); 
    }
    double rho = Rho_fixedGridRhoFastjetAll;
    double Pileup_nTrue = Pileup_nTrueInt;
    double NPV = PV_npvs;
    double NPV_Good = PV_npvsGood;

    bool doPtHatFilter = true; // Set to false for MC Nano V09, isMG works fine 
    if (doPtHatFilter && isMC )
    {
      if (isMG && !isRun3 && 2. * Pileup_pthatmax > LHE_HT)
        continue;
      if (isMG && isRun3 && 2. * Jet_pt[0] / LHE_HT > 2.5 / pow(LHE_HT / 40., 2) + 1.5)
        continue; // Run3 MG patch for missing Pileup_pthatmax
      if (!isMG && Pileup_pthatmax > Generator_binvar)
        continue;
    }

    if (debugevent)
      cout << "Keep track of run+LS" << endl
           << flush;

    if (mrunls.find(run) == mrunls.end())
      ++nrun;
    if (mrunls[run].find(luminosityBlock) == mrunls[run].end())
      ++nls;
    ++nevt;
    // THIS HAS BEEN MOVED TO LUMI CALCULATION
    // mrunls[run][luminosityBlock] = 1;

    // Check that MET filters are all true
    bool pass_METfilter = ((isRun3 || isRun2) &&
		           //MET_pt / MET_sumEt > 0.3 && // To (maybe) remove noise spoiling the RMS. Nestor April24, 2024.
		           !(RawPuppiMET_pt / RawPuppiMET_sumEt > 0.3) && // To (maybe) remove noise spoiling the RMS. Nestor April24, 2024.
                           Flag_goodVertices &&
                           Flag_globalSuperTightHalo2016Filter &&
                           Flag_EcalDeadCellTriggerPrimitiveFilter && //
                           Flag_BadPFMuonFilter && //
                           Flag_BadPFMuonDzFilter && //
                           Flag_hfNoisyHitsFilter && //
                           Flag_eeBadScFilter &&
                           Flag_ecalBadCalibFilter);

    // Check if any triggers fired and make histogram of them
    if (doTrigger)
    {

      if (debugevent)
        cout << "Check trigger" << endl
             << flush;

      bool fired = false;
      for (int i = 0; i != ntrg; ++i)
      {
        fired = (fired || (*mtrg[vtrg[i]]));
        if (*mtrg[vtrg[i]])
          htrg->Fill(i);
      }
      if (!fired)
      {
        ++_nbadevts_trg;
        continue;
      }
    } // doTrigger
    ++_ngoodevts;

    if (debugevent)
      cout << "Redo JEC" << endl
           << flush;

    // Redo JEC right after event cuts but before anything else
    // Do not re-sort (for now)
    bool allJetsGood(true);
    int njet = nJet;

    //Nestor Aug16, 2024.
    //Search for two jets at about 4.2 TeV in 2024F, at two different |eta| in barrel.
    if (Jet_pt[0] > 4000 && Jet_pt[1] > 4000 && fabs(Jet_eta[0]) < 1.3 && fabs(Jet_eta[1]) < 1.3)
    {
      std::cout << "Event: " << event << ", LS: " << luminosityBlock << ", Run: " << run << std::endl;
      std::cout << "Jet0 pT: " << Jet_pt[0] << "Jet1 pT: " << Jet_pt[1] << std::endl;
    }
    //
    
    for (int i = 0; i != njet; ++i)
    {

      /*
      //Nestor Aug16, 2024.
      //Search for two jets at about 4.2 TeV in 2024F, at two different |eta| in barrel.
      if (Jet_pt[0] > 4000 && Jet_pt[1] > 4000 && fabs(Jet_eta[0]) < 1.3 && fabs(Jet_eta[1]) < 1.3 && Jet_eta[0] != Jet_eta[1])
      {
        std::cout << "Event: " << event << ", LS: " << luminosityBlock << ", Run: " << run << std::endl;
	std::cout << "Jet0 pT: " << Jet_pt[0] << "Jet1 pT: " << Jet_pt[1] << std::endl;
      }
      */

      if (redoJEC)
      {
        double rawJetPt = Jet_pt[i] * (1.0 - Jet_rawFactor[i]);
        double rawJetMass = Jet_mass[i] * (1.0 - Jet_rawFactor[i]);
        jec->setJetPt(rawJetPt);
        jec->setJetEta(Jet_eta[i]);
        jec->setJetPhi(Jet_phi[i]);
        if (isRun2)
        {
          jec->setJetA(Jet_area[i]);
          jec->setRho(Rho_fixedGridRhoFastjetAll);
          jecl1rc->setJetPt(rawJetPt);
          jecl1rc->setJetEta(Jet_eta[i]);
          jecl1rc->setJetA(Jet_area[i]);
          jecl1rc->setRho(Rho_fixedGridRhoFastjetAll);
        }
        // double corr = jec->getCorrection();
        vector<float> v = jec->getSubCorrections();
        double corr = v.back();
        double res = (v.size() > 1 ? v[v.size() - 1] / v[v.size() - 2] : 1.);
        Jet_RES[i] = 1. / res;
        Jet_deltaJES[i] = (1. / corr) / (1.0 - Jet_rawFactor[i]);
        Jet_pt[i] = corr * rawJetPt;
        Jet_mass[i] = corr * rawJetMass;
        Jet_rawFactor[i] = (1.0 - 1.0 / corr);
        // pt*(1-l1rcFactor)=ptl1rc => l1rcFactor = 1 - ptl1rc/pt
        Jet_l1rcFactor[i] = (isRun2 ? (1.0 - jecl1rc->getCorrection() / corr) : Jet_rawFactor[i]);
      }
      else
      {
        Jet_RES[i] = 1.;
        Jet_deltaJES[i] = 1.;
        Jet_l1rcFactor[i] = Jet_rawFactor[i];
	//Jet_l1rcFactor[i] = Jet_pt[i] * (1.0 - Jet_rawFactor[i]); //To test raw pt response, Nestor. April 16, 2024.
      }
      
      bool dojv_andBPix = false; // Nestor. Aug15, 2024 
      if (true)//Nestor: changed March 27,2024
      { // check jet veto
        int i1 = h2jv->GetXaxis()->FindBin(Jet_eta[i]);
        int j1 = h2jv->GetYaxis()->FindBin(Jet_phi[i]);
        //int i2 = h2jvBPix->GetXaxis()->FindBin(Jet_eta[i]);
        //int j2 = h2jvBPix->GetYaxis()->FindBin(Jet_phi[i]);
        Jet_jetvetomap[i] = (h2jv->GetBinContent(i1, j1) > 0);
	//Jet_jetveto_BPix[i] = (h2jvBPix->GetBinContent(i2, j2) > 0);

	if (dojv_andBPix)
	{
	  Jet_jetveto[i] = Jet_jetvetomap[i] || Jet_jetveto_BPix[i]; 
	  //Jet_jetveto[i] = (h2jv->GetBinContent(i1, j1) > 0);
	}
	else {
	  //cout << "Using jetvetomap all" << endl;
          Jet_jetveto[i] = (h2jv->GetBinContent(i1, j1) > 0);
	}
      } // jet veto
      else
        Jet_jetveto[i] = false;

      // Fail allJetsGood flag if any jet of pT>15 is not good
      if (!(Jet_jetId[i] >= 4 && !Jet_jetveto[i]) && Jet_pt[i] > 15.)
        allJetsGood = false;
      // NB: should move this after smearing. Separate loop for type-I MET?
    } // for njet

    // Apply JER smearing to MC immediately after JEC. Don't change order.
    // Need after JEC to ensure mean is at 1, but ideally should recalculate
    // JEC with smeared reco pT for consistency
    // Jet_smearFactor[i] = 0.;
    // Safety reset that works for data as well
    for (int i = 0; i != njet; ++i)
    {
      Jet_CF[i] = 1.;
    } // reset Jet_CF


/*
    std::string triggerString(double pt, double eta) {
        std::map<std::string, range> *triggers;
        if (analysis == "ismultijet") triggers = &mi;
        else if (analysis == "isdijet") triggers = &md;
        else if (analysis == "isdijet2") triggers = &md2;
        else {
            std::cerr << "Unknown analysis type: " << analysis << std::endl;
            return "HLT_ZeroBias";
        }

        for (const auto &entry : *triggers) {
            const std::string &trigger = entry.first;
            const range &r = entry.second;
            if (pt > r.pt_min && pt <= r.pt_max && eta >= r.eta_min && eta <= r.eta_max) {
                return trigger;
            }
        }

    return "HLT_ZeroBias"; // Default
    } 


*/
    // reweightPU
    
    
    if (isMC && reweightPU && !doPU_per_trigger)
    {
      assert(pileupRatio);
      // Get the bin
      int ibin = pileupRatio->FindBin(Pileup_nTrueInt);
      double pileup_weight = pileupRatio->GetBinContent(ibin);
      w *= pileup_weight;

      //std::cerr << "pileup_weight original: " << pileup_weight << std::endl;
      //get_weight(700., 2., "doMultijets");
      //std::cerr << "PU_weight using the function: " << PU_weight <<std::endl;
    }
    
    if (do_PUProfiles){
       if (isMC){
          h_PUProfile->Fill(Pileup_nTrue, w);
       }
       //h_PUProfile->Fill(Pileup_nTrue, w);
       h_RhoAll->Fill(rho, w);
       h_NPV->Fill(NPV, w);
       h_NPVGood->Fill(NPV_Good, w);
    }


    if (isMC && smearJets)
    {

      for (int i = 0; i != njet; ++i)
      {

        Jet_CF[i] = 1.;
        if (i < smearNMax)
        {

          // Retrieve genJet and calculate dR
          double dR(999);
          p4.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);

          if (Jet_genJetIdx[i] >= 0 )// && Jet_genJetIdx[i] < nGenJet)
          {
            int j = Jet_genJetIdx[i];
            p4g.SetPtEtaPhiM(GenJet_pt[j], GenJet_eta[j], GenJet_phi[j],
                             GenJet_mass[j]);
            dR = p4g.DeltaR(p4);
          }
          else
            p4g.SetPtEtaPhiM(0, 0, 0, 0);

          // Rename variables to keep naming as in jetphys/IOV.h.
          double jPt = Jet_pt[i];
          double jEta = Jet_eta[i];
          double rho = Rho_fixedGridRhoFastjetAll;
          double jE = p4.E();
          double jPtGen = p4g.Pt();
          // Set constants
          double MIN_JET_ENERGY = 0.01; // TBD

          // Some problems with the code below:
          // 1) JER should us primarily genPt, secondary recoPt
          // 2) relDpt  should evaluate vs genPt to avoid <1/x> != 1/<x> bias
          // 3) For (JME)NANO, should also check DR of genJet
          // Probably small impact except for the last, which I add

          // The method presented here can be found in https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution
          // and the corresponding code in https://github.com/cms-sw/cmssw/blob/CMSSW_8_0_25/PhysicsTools/PatUtils/interface/SmearedJetProducerT.h
          assert(jer);
          double Reso = jer->getResolution({{JME::Binning::JetPt, jPt}, {JME::Binning::JetEta, jEta}, {JME::Binning::Rho, rho}});
          double SF(1);
          if (useJERSFvsPt && jersfvspt)
          {
            jersfvspt->setJetEta(jEta);
            jersfvspt->setJetPt(jPt);
            jersfvspt->setRho(rho);
            SF = jersfvspt->getCorrection();
          }
          else if (!useJERSFvsPt && jersf)
          {
            SF = jersf->getScaleFactor({{JME::Binning::JetEta, jEta}, {JME::Binning::Rho, rho}}, Variation::NOMINAL);
          }
          else
          {
            cout << "No JER SF available" << endl << flush;
            assert(false);
          }

          // Case 0: by default the JER correction factor is equal to 1
          double CF = 1.;
          // We see if the gen jet meets our requirements
          bool condPt = (jPtGen > MIN_JET_ENERGY && dR < 0.2);
          double relDPt = condPt ? (jPt - jPtGen) / jPt : 0.0;
          bool condPtReso = fabs(relDPt) < 3 * Reso;
          if (condPt and condPtReso)
          {
            // Case 1: we have a "good" gen jet matched to the reco jet (indicated by positive gen jet pt)
            CF += (SF - 1.) * relDPt;
          }
          else if (SF > 1)
          {
            // Case 2: we don't have a gen jet. Smear jet pt using a random gaussian variation
            double sigma = Reso * std::sqrt(SF * SF - 1);
            std::normal_distribution<> d(0, sigma);
            CF += d(_mersennetwister);
          }

          // Negative or too small smearFactor. Safety precautions.
          double CFLimit = MIN_JET_ENERGY / jE;
          if (CF < CFLimit)
            CF = CFLimit;

          double origPt = Jet_pt[i];
          double origJetMass = Jet_mass[i];
          Jet_pt[i] = CF * origPt;
          Jet_mass[i] = CF * origJetMass;
          Jet_CF[i] = CF;
          // Jet_smearFactor[i] = (1.0 - 1.0/CF);

          // type-I calculation is done later and propagates JER SF
          // (needs to have unsmeard p4lr1c, fully smeared p4)
        } // i<smearNMax
      }   // for njet
    }     // JER smearing

    // Calculate MC truth right after JEC and smearing to test closure
    if (isMC && doMCtruth)
    {

      mctruthHistos *h = mhmc["HLT_MC"];

      // First, map reco->gen so can quickly invert gen->reco
      // Also reset dR
      map<int, int> genToReco;
      for (int i = 0; i != njet; ++i)
      {
        if (Jet_genJetIdx[i] >= 0)
        {
          genToReco[Jet_genJetIdx[i]] = i;
        }
        Jet_genDR[i] = 999.;
        h->h2pteta_rec->Fill(fabs(Jet_eta[i]), Jet_pt[i], w);
      } // for i

      // Then loop over genjets and also update dr
      for (Int_t j = 0; j != nGenJet; ++j)
      {

        p4g.SetPtEtaPhiM(GenJet_pt[j], GenJet_eta[j], GenJet_phi[j],
                         GenJet_mass[j]);
        double dR(999);
        int i(-1);
        if (genToReco.find(j) != genToReco.end())
        {
          i = genToReco[j];
          p4.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
          dR = p4g.DeltaR(p4);
          Jet_genDR[i] = dR;
        }
        else
          p4.SetPtEtaPhiM(0, 0, 0, 0);

	if (i<3)
        {
          h->ptreco_ptgen->Fill(p4.Pt()/p4g.Pt(), w);
	  h->h2res_ptgen->Fill(p4g.Pt(), p4.Pt()/p4g.Pt(), w);
	  h->h2_btagUpar->Fill(p4g.Pt(), Jet_btagPNetQvG[i], w);
	  h->h2res_etagen->Fill(p4g.Eta(), p4.Pt()/p4g.Pt(), w);
	  h->h3res->Fill(p4g.Eta(), p4g.Pt(), p4.Pt()/p4g.Pt(), w);
	  h->p2r_NoMatch->Fill(fabs(p4.Eta()), p4g.Pt(), p4.Pt() / p4g.Pt(), w);
	  //if (abs(p4g.Eta()) < 0.5 && p4g.Pt() >= 97 && p4g.Pt() < 114)
	  //{
	  //  h->h1res_bar->Fill(p4.Pt()/p4g.Pt(), w);
	  //}
        }
        h->h2pteta_gen->Fill(fabs(p4g.Eta()), p4g.Pt(), w);
        bool hasMatchVtx = (fabs(PV_z - GenVtx_z) < 0.2);
        bool hasMatchJet = (dR < 0.2 && p4g.Pt() > 0 && p4.Pt() > 0);
        if (hasMatchVtx && hasMatchJet)
        {
          h->h2pteta->Fill(fabs(p4.Eta()), p4g.Pt(), w);
          h->p2jes->Fill(fabs(p4.Eta()), p4g.Pt(), (1. - Jet_rawFactor[i]), w);
          h->p2jsf->Fill(fabs(p4.Eta()), p4g.Pt(),
                         smearJets ? Jet_CF[i] : 1, w);
          h->h3res_Match->Fill(p4g.Eta(), p4g.Pt(), p4.Pt()/p4g.Pt(), w);
	  h->h3res_raw->Fill(p4g.Eta(), p4g.Pt(), Jet_pt[i] * (1.0 - Jet_rawFactor[i]) / p4g.Pt(), w);
	  h->p2r->Fill(fabs(p4.Eta()), p4g.Pt(), p4.Pt() / p4g.Pt(), w);
          h->p2r_raw->Fill(fabs(p4.Eta()), p4g.Pt(), Jet_pt[i] * (1.0 - Jet_rawFactor[i]) / p4g.Pt(), w);
        }
        h->p2effz->Fill(fabs(p4g.Eta()), p4g.Pt(), hasMatchVtx ? 1 : 0, w);
        if (hasMatchVtx)
          h->p2eff->Fill(fabs(p4g.Eta()), p4g.Pt(), hasMatchJet ? 1 : 0, w);
      } // for j

      // Finally check fake rates
      for (int i = 0; i != njet; ++i)
      {
        bool hasMatchVtx = (fabs(PV_z - GenVtx_z) < 0.2);
        bool hasMatchJet = (Jet_genDR[i] < 0.2);
        if (hasMatchVtx)
          h->p2pur->Fill(fabs(Jet_eta[i]), Jet_pt[i], hasMatchJet ? 1 : 0);
      } // for i

    } // isMC && doMCtruth

    if (debugevent)
      cout << "Sum four-vectors for MHT" << endl
           << flush;

    // int njet3 = 0;
    // int njetn = 0;

    // Reset MET vectors
    if (isRun2)
    {
      p4rawmet.SetPtEtaPhiM(ChsMET_pt, 0, ChsMET_phi, 0);
      p4t1met.SetPtEtaPhiM(ChsMET_pt, 0, ChsMET_phi, 0);
      p4m0.SetPtEtaPhiM(ChsMET_pt, 0, ChsMET_phi, 0);
    }
    else
    {
      p4rawmet.SetPtEtaPhiM(RawPuppiMET_pt, 0, RawPuppiMET_phi, 0);
      p4t1met.SetPtEtaPhiM(RawPuppiMET_pt, 0, RawPuppiMET_phi, 0);
      p4m0.SetPtEtaPhiM(RawPuppiMET_pt, 0, RawPuppiMET_phi, 0);
      // p4rawmet.SetPtEtaPhiM(PuppiMET_pt,0,PuppiMET_phi,0);
      // p4t1met.SetPtEtaPhiM(PuppiMET_pt,0,PuppiMET_phi,0);
      // p4m0.SetPtEtaPhiM(PuppiMET_pt,0,PuppiMET_phi,0);
    }
    p4mht.SetPtEtaPhiM(0, 0, 0, 0);

    // Reset dijet vectors
    p4m2.SetPtEtaPhiM(0, 0, 0, 0);
    p4mn.SetPtEtaPhiM(0, 0, 0, 0);
    p4mu.SetPtEtaPhiM(0, 0, 0, 0);

    // Reset multijet vectors
    // bool ismultijet = (njet>=3); // multijet pre-setting
    p4lead.SetPtEtaPhiM(0, 0, 0, 0);
    p4recoil.SetPtEtaPhiM(0, 0, 0, 0);
    p4m3.SetPtEtaPhiM(0, 0, 0, 0);
    p4mn3.SetPtEtaPhiM(0, 0, 0, 0);
    p4mu3.SetPtEtaPhiM(0, 0, 0, 0);
    p4leadRES.SetPtEtaPhiM(0, 0, 0, 0);
    p4recoilRES.SetPtEtaPhiM(0, 0, 0, 0);
    int nlead(0);
    int nrecoil(0);
    bool multijet_vetonear(false);
    bool multijet_vetofwd(false);

    for (int i = 0; i != njet; ++i)
    {

      // p4 is fully corrected and smeared
      p4.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
      // p4raw.SetPtEtaPhiM(Jet_pt[i]*(1.0-Jet_rawFactor[i]), Jet_eta[i],
      //		   Jet_phi[i], Jet_mass[i]*(1.0-Jet_rawFactor[i]));
      // p4l1rc.SetPtEtaPhiM(Jet_pt[i]*(1.0-Jet_l1rcFactor[i]), Jet_eta[i],
      //		    Jet_phi[i], Jet_mass[i]*(1.0-Jet_l1rcFactor[i]));
      //  p4l1rc is before corrections and smearing
      p4l1rc.SetPtEtaPhiM(Jet_pt[i] / (smearJets && Jet_CF[i] ? Jet_CF[i] : 1) * (1.0 - Jet_l1rcFactor[i]),
                          Jet_eta[i], Jet_phi[i],
                          Jet_mass[i] / (smearJets && Jet_CF[i] ? Jet_CF[i] : 1) * (1.0 - Jet_l1rcFactor[i]));

      // Jet veto maps
      if (doJetveto)
      {

        for (int itrg = 0; itrg != ntrg; ++itrg)
        {
          string &trg = vtrg[itrg];
          if (!(*mtrg[trg]))
            continue;

          jetvetoHistos *h = mhjv[trg];
          assert(h);
          double abseta = fabs(p4.Eta());
          double pt = p4.Pt();

          h->h2pteta_all->Fill(p4.Eta(), p4.Pt(), w);
          // Debug
          if (Jet_jetId[i] >= 4 && pass_METfilter > 0 &&
              pt >= h->ptmin && pt < h->ptmax &&
              abseta >= h->absetamin && abseta < h->absetamax)
          {

            h->h2pteta_sel->Fill(p4.Eta(), p4.Pt(), w);
            h->h2phieta->Fill(p4.Eta(), p4.Phi(), w);
            if (doPFComposition)
            {
              h->p2chf->Fill(p4.Eta(), p4.Phi(), Jet_chHEF[i], w);
              h->p2nef->Fill(p4.Eta(), p4.Phi(), Jet_neEmEF[i], w);
              h->p2nhf->Fill(p4.Eta(), p4.Phi(), Jet_neHEF[i], w);
            }
          }
        }
      } // doJetveto

      // Inclusive jets
      if (doIncjet)
      {
        for (int itrg = 0; itrg != ntrg; ++itrg)
        {

          string &trg = vtrg[itrg];
          if (!(*mtrg[trg]))
            continue;

          incjetHistos *h = mhij[trg];

          h->h2pteta_all->Fill(p4.Eta(), p4.Pt(), w);
          if (Jet_jetId[i] >= 4 && !Jet_jetveto[i] && pass_METfilter > 0)
          {

	    //std::cout << run << " Inclusive h2pteta weight: " << w << std::endl;
            h->h2pteta->Fill(p4.Eta(), p4.Pt(), w);

	    if (p4.Pt()>20){
	      h->hpteta20->Fill(p4.Eta(), w);
	    }
            if (p4.Pt()>30){
              h->hpteta30->Fill(p4.Eta(), w);
            }
            if (p4.Pt()>40){
              h->hpteta40->Fill(p4.Eta(), w);
            }
            if (p4.Pt()>50){
              h->hpteta50->Fill(p4.Eta(), w);
            }

	    if (mlumi[trg][run] > 0){
	      h->h2pteta_lumi->Fill(p4.Eta(), p4.Pt(), 1./mlumi[trg][run]);
            }

            if (isMG && i == 0)
            {
              h->h2jtvht->Fill(LHE_HT, 2 * p4.Pt(), w);
              h->h2jtoht->Fill(LHE_HT, 2 * p4.Pt() / LHE_HT, w);
            }

            if (p4.Pt() >= h->ptmin && p4.Pt() < h->ptmax &&
                fabs(p4.Rapidity()) > h->absetamin &&
                fabs(p4.Rapidity()) < h->absetamax)

              h->h2pteta_sel->Fill(p4.Eta(), p4.Pt(), w);

            if (fabs(p4.Rapidity()) < 1.3){
	      //std::cout << run << " Inclusive hpt13 weight: " << w << std::endl;
              h->hpt13->Fill(p4.Pt(), w);
	    }
            int iy = int(fabs(p4.Rapidity()) / 0.5);
            if (iy < h->ny)
              h->vpt[iy]->Fill(p4.Pt(), w);

            if (doPFComposition)
            {
              double eta = p4.Eta();
              double pt = p4.Pt();
              h->p2pt->Fill(eta, pt, Jet_pt[i], w);
	      //
	      if (doPU_per_trigger){
	         get_weight(pt, eta, "doInc");
	      }
              h->p2rho_PURW->Fill(eta, pt, rho, w * PU_weight);
	      //
	      h->p2rho->Fill(eta, pt, rho, w);
              h->p2chf->Fill(eta, pt, Jet_chHEF[i], w);
              h->p2nhf->Fill(eta, pt, Jet_neHEF[i], w);
              h->p2nef->Fill(eta, pt, Jet_neEmEF[i], w);
              h->p2cef->Fill(eta, pt, Jet_chEmEF[i], w);
              h->p2muf->Fill(eta, pt, Jet_muEF[i], w);

              if (fabs(eta) < 1.3)
              {
                h->ppt13->Fill(pt, Jet_pt[i], w);
                h->prho13->Fill(pt, rho, w);
                h->pchf13->Fill(pt, Jet_chHEF[i], w);
		h->h2chf13->Fill(pt, Jet_chHEF[i], w);
                h->pnhf13->Fill(pt, Jet_neHEF[i], w);
                h->pnef13->Fill(pt, Jet_neEmEF[i], w);
                h->pcef13->Fill(pt, Jet_chEmEF[i], w);
                h->pmuf13->Fill(pt, Jet_muEF[i], w);
              }
            } // doPFcomposition
          }   // JetID+METfilter
        }     // for itrg
      }       // doIncJet

    /*
    //doJetsperRun
      if (doJetsperRuns && dolumi)
      {
        auto it5 = std::find(_runNumberBin.begin(), _runNumberBin.end(), run);
        for (int itrg = 0; itrg != ntrg; ++itrg)
        {
          string &trg = vtrg[itrg];
          //std::cout << "Trigger: " << trg << std::endl;
          if (!(*mtrg[trg]))
            continue;
          jetsperRuns *h = mjet[trg];
          //const range &r = mt[trg];

          p4.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
          if (Jet_jetId[i] >= 4 && !Jet_jetveto[i] && pass_METfilter > 0 ) //&& fabs(p4.Eta()) < 1.3)  
          {

            w = (isMC ? genWeight : 1.);
	    h->h1jetrate->Fill(run, w);
            if (it5 != _runNumberBin.end() && mlumi[trg][run] > 0){
              //std::cout << run << " is included in runNumberBin and the rec luminosity is: " << _lums[run] << std::endl;
              w = (isMC ? genWeight : 1./mlumi[trg][run]);
            }
            else {
              //w = (isMC ? genWeight : 1.);
	      //exit(0);
	      continue;
            }
	    if (p4.Pt() >= h->ptmin && p4.Pt() < h->ptmax &&
	        fabs(p4.Rapidity()) > h->absetamin &&
		fabs(p4.Rapidity()) < h->absetamax) {
                  //std::cout << "The p4.pt is: " << p4.Pt() << " and the pt min is: " << r.ptmin << " from the trg: " << itrg << " weight: " << w << " and the itrg is: " << itrg << std::endl;
                h->h1jetxsec->Fill(run, w);
                h->h2jetpteta->Fill(fabs(p4.Eta()), p4.Pt(), w);
		h->p2MPF->Fill(ptlead, run, m0l, w);
		if (fabs(p4.Eta()) < 1.3) {
                  h->h1pt13_w->Fill(p4.Pt(), w);
		  h->h1pt13->Fill(p4.Pt(), 1.);
		}
            }
          }
        }
        w = (isMC ? genWeight : 1.);
      } // doJetsperRun
      */

      // Calculate type-I MET (L1L2L3-RC) and MHT
      if (p4.Pt() > 15.)
      {
        p4mht -= p4;
        p4t1met += p4l1rc - p4; // same as (+raw-rcoff) -corr,
        p4m0 += p4l1rc - p4;    // same as (+raw-rcoff) -corr
      }

      // L2Res HDM (dijet)
      if (i < 2 && p4.Pt() > 15.)
      { // two leading jets
        p4m2 -= p4;
      }
      else if (p4.Pt() > 15.)
      {
        p4mn -= p4;
      }

      // L3Res HDM (multijet)
      if (i == 0 && p4.Pt() > 30.)
      { // leading jet
        p4lead += p4;
        p4m3 -= p4;
        p4leadRES += Jet_RES[i] * p4;
        ++nlead;
      }
      else if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) < 2.5 &&
               DELTAPHI(p4.Phi(), p4lead.Phi()) > 1.0)
      { // recoil jets
        p4recoil += p4;
        p4m3 -= p4;
        p4recoilRES += Jet_RES[i] * p4;
        ++nrecoil;
      }
      else if (p4.Pt() > 15.)
      { // all other jets
        p4mn3 -= p4;
      }

      // Veto nearby jets for multijet topology
      if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) < 2.5 &&
          DELTAPHI(p4.Phi(), p4lead.Phi()) <= 1.0)
        multijet_vetonear = true;

      // Veto forward jets for multijet topology
      if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) >= 2.5)
        multijet_vetofwd = true;

    } // for i in njet

    // Calculate unclustered MET from the remainders
    // met = -j2 -jn -ju = m2 + mn + mu => mu = met -m2 -mn
    p4mu = p4m0 - p4m2 - p4mn;
    p4mu3 = p4m0 - p4m3 - p4mn3;

    // Also check recoil phi for multijet selection
    double ptrecoil = p4recoil.Pt();
    double dphirecoil = DELTAPHI(p4lead.Phi(), p4recoil.Phi());
    // Use tightLepVeto for JetID
    bool ismultijet =
        (nlead == 1 && nrecoil >= 2 && !multijet_vetonear && !multijet_vetofwd &&
         fabs(dphirecoil - TMath::Pi()) < 0.3 && pass_METfilter > 0 &&
         Jet_pt[0] > 30. && fabs(Jet_eta[0]) < 1.3 && Jet_jetId[0] >= 4 &&
         Jet_pt[1] > 30. && fabs(Jet_eta[1]) < 2.5 && Jet_jetId[1] >= 4 &&
         Jet_pt[2] > 30. && fabs(Jet_eta[2]) < 2.5 && Jet_jetId[2] >= 4 &&
         !Jet_jetveto[0] && !Jet_jetveto[1] && !Jet_jetveto[2] &&
         Jet_pt[1] < 0.6 * ptrecoil && Jet_pt[2] < 0.6 * ptrecoil);

    // Calculate Crecoil
    double logCrecoil(0);
    double ptavp3(0);
    if (ismultijet && doMultijet)
    {

      // Proper bisector axis (equal angles to each jet)
      p4b3.SetPtEtaPhiM(0, 0, 0, 0);
      p4b3r.SetPtEtaPhiM(1, 0, p4recoil.Phi(), 0);
      p4b3l.SetPtEtaPhiM(1, 0, p4lead.Phi(), 0);
      p4b3 += p4b3r;
      p4b3 -= p4b3l;
      p4b3.SetPtEtaPhiM(p4b3.Pt(), 0., p4b3.Phi(), 0.);
      p4b3 *= 1. / p4b3.Pt();

      // Average projection pT to bisector axis, pT,avp
      // as explained in JME-21-001 (HDM method: bisector extension)
      ptavp3 = 0.5 * (p4recoil.Vect().Dot(p4b3.Vect()) -
                      p4lead.Vect().Dot(p4b3.Vect()));

      double ptlead = p4lead.Pt();
      double ptave = 0.5 * (ptlead + ptrecoil);
      for (int i = 0; i != njet; ++i)
      {

        // Crecoil = exp(sum_i F_i log(f_i)), where
        // f_i = pT,i / pTrecoil, F_i = f_i cos(Delta phi(i,recoil))
        // To do this before calculating pTrecoil, we could do
        // sum_i pT,i * cos(Delta phi(i,-lead))* log(pT,i / pT,lead)
        // which should for practical purposes be the same
        // Maybe safer here as originally defined, just careful with selection

        // Make sure selection here matches the one above for p4recoil
        p4.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
        if (i > 0 && p4.Pt() > 30. && fabs(p4.Eta()) < 2.5 &&
            DELTAPHI(p4.Phi(), p4lead.Phi()) > 1.0)
        {
          double pti = p4.Pt();
          double fi = pti / ptrecoil;
          double Fi = fi * cos(DELTAPHI(p4.Phi(), p4recoil.Phi()));
          logCrecoil += Fi * log(fi);

          if (doMultijet2Drecoil)
          {

            for (int itrg = 0; itrg != ntrg; ++itrg)
            {

              string &trg = vtrg[itrg];
              if (!(*mtrg[trg]))
                continue;

              multijetHistos *h = mhmj[trg];

              // Assumption is that sum_i F_i = 1, but should check?
              h->h2recoila->Fill(ptavp3, pti, w * Fi);
              h->h2recoilm->Fill(ptave, pti, w * Fi);
              h->h2recoill->Fill(ptlead, pti, w * Fi);
              h->h2recoilr->Fill(ptrecoil, pti, w * Fi);

              if (doPFComposition)
              {
                h->ppt25->Fill(ptavp3, Jet_pt[i], w * Fi);
                h->prho25->Fill(ptavp3, rho, w * Fi);
                h->pchf25->Fill(ptavp3, Jet_chHEF[i], w * Fi);
                h->pnhf25->Fill(ptavp3, Jet_neHEF[i], w * Fi);
                h->pnef25->Fill(ptavp3, Jet_neEmEF[i], w * Fi);
                h->pcef25->Fill(ptavp3, Jet_chEmEF[i], w * Fi);
                h->pmuf25->Fill(ptavp3, Jet_muEF[i], w * Fi);
              } // doPFcomposition

            } // for itrg
          }   // doMultijet2Drecoil
        }     // good recoil jet
      }       // for i in injet
    }         // doMultijet
    double Crecoil = exp(logCrecoil);

    hnjet->Fill(njet, w);

    // Dijet pre-selection
    if (njet >= 2)
    {

      if (debugevent)
        cout << "Dijet analysis" << endl
             << flush;

      // Both leading jets act as tag and probe in turn
      for (int itag = 0; itag != 2; ++itag)
      {

        // Tag and probe jet selection
        int iprobe = (itag == 0 ? 1 : 0);
        p4t.SetPtEtaPhiM(Jet_pt[itag], Jet_eta[itag], Jet_phi[itag],
                         Jet_mass[itag]);
        p4p.SetPtEtaPhiM(Jet_pt[iprobe], Jet_eta[iprobe], Jet_phi[iprobe],
                         Jet_mass[iprobe]);

        // Dijet observables
        double eta = p4p.Eta();
        double pttag = p4t.Pt();
        double ptprobe = p4p.Pt();
        double ptave = 0.5 * (pttag + ptprobe);
        double asymm = (ptprobe - pttag) / ptave;

        double dphi = DELTAPHI(p4t.Phi(), p4p.Phi());
        double dr = p4t.DeltaR(p4p);

        // Proper bisector axis (equal angles to each jet)
        p4b.SetPtEtaPhiM(0, 0, 0, 0);
        p4bt.SetPtEtaPhiM(1, 0, p4t.Phi(), 0);
        p4bp.SetPtEtaPhiM(1, 0, p4p.Phi(), 0);
        p4b += p4bt;
        p4b -= p4bp;
        p4b.SetPtEtaPhiM(p4b.Pt(), 0., p4b.Phi(), 0.);
        p4b *= 1. / p4b.Pt();
        p4bx.SetPtEtaPhiM(p4b.Pt(), 0., p4b.Phi() + 0.5 * TMath::Pi(), 0.);

        // Average projection pT to bisector axis, pT,avp
        // as explained in JME-21-001 (HDM method: bisector extension)
        double ptavp2 = 0.5 * (p4t.Vect().Dot(p4b.Vect()) -
                               p4p.Vect().Dot(p4b.Vect()));

        double m0b = 1 + (p4m0.Vect().Dot(p4b.Vect())) / ptavp2;
        double m2b = 1 + (p4m2.Vect().Dot(p4b.Vect())) / ptavp2;
        double mnb = 0 + (p4mn.Vect().Dot(p4b.Vect())) / ptavp2;
        double mub = 0 + (p4mu.Vect().Dot(p4b.Vect())) / ptavp2;
        // double mob = 0 + (p4mo.Vect().Dot(p4b.Vect()))/ptavp;

        double m0bx = 1 + (p4m0.Vect().Dot(p4bx.Vect())) / ptavp2;
        double m2bx = 1 + (p4m2.Vect().Dot(p4bx.Vect())) / ptavp2;

        // Extras
        double cu = 1. / 0.92;
        double mnub = 0 + ((p4mn + cu * p4mu).Vect().Dot(p4b.Vect())) / ptavp2;
        double mnbx = 0 + (p4mn.Vect().Dot(p4bx.Vect())) / ptavp2;
        double mubx = 0 + (p4mu.Vect().Dot(p4bx.Vect())) / ptavp2;
        double mnubx = 0 + ((p4mn + cu * p4mu).Vect().Dot(p4bx.Vect())) / ptavp2;

        // bisector axis => dijet axis really (not equal angles)
        p4d.SetPtEtaPhiM(0, 0, 0, 0);
        p4d += p4t;
        p4d -= p4p;
        p4d.SetPtEtaPhiM(p4d.Pt(), 0., p4d.Phi(), 0.);
        p4d *= 1. / p4d.Pt();
        p4dx.SetPtEtaPhiM(p4d.Pt(), 0., p4d.Phi() + 0.5 * TMath::Pi(), 0.);

        double m0d = 1 + (p4m0.Vect().Dot(p4d.Vect())) / ptave;
        double m2d = 1 + (p4m2.Vect().Dot(p4d.Vect())) / ptave;
        double mnd = 0 + (p4mn.Vect().Dot(p4d.Vect())) / ptave;
        double mud = 0 + (p4mu.Vect().Dot(p4d.Vect())) / ptave;
        // double mod = 0 + (p4mo.Vect().Dot(p4d.Vect()))/ptave;

        double m0dx = 1 + (p4m0.Vect().Dot(p4dx.Vect())) / ptave;
        double m2dx = 1 + (p4m2.Vect().Dot(p4dx.Vect())) / ptave;

        // central axis + pttag norm&binning
        p4c.SetPtEtaPhiM(0, 0, 0, 0);
        p4c += p4t;
        p4c.SetPtEtaPhiM(p4c.Pt(), 0., p4c.Phi(), 0.);
        p4c *= 1. / p4c.Pt();
        p4cx.SetPtEtaPhiM(p4c.Pt(), 0., p4c.Phi() + 0.5 * TMath::Pi(), 0.);

        double m0c = 1 + (p4m0.Vect().Dot(p4c.Vect())) / pttag;
        double m2c = 1 + (p4m2.Vect().Dot(p4c.Vect())) / pttag;
        double mnc = 0 + (p4mn.Vect().Dot(p4c.Vect())) / pttag;
        double muc = 0 + (p4mu.Vect().Dot(p4c.Vect())) / pttag;
        // double moc = 0 + (p4mo.Vect().Dot(p4c.Vect()))/pttag;

        double m0cx = 1 + (p4m0.Vect().Dot(p4cx.Vect())) / pttag;
        double m2cx = 1 + (p4m2.Vect().Dot(p4cx.Vect())) / pttag;

        // forward axis ("backwards" to tag hemisphere) + ptprobe norm&binning
        p4f.SetPtEtaPhiM(0, 0, 0, 0);
        p4f -= p4p;
        p4f.SetPtEtaPhiM(p4f.Pt(), 0., p4f.Phi(), 0.);
        p4f *= 1. / p4f.Pt();
        p4fx.SetPtEtaPhiM(p4f.Pt(), 0., p4f.Phi() + 0.5 * TMath::Pi(), 0.);

        double m0f = 1 + (p4m0.Vect().Dot(p4f.Vect())) / ptprobe;
        double m2f = 1 + (p4m2.Vect().Dot(p4f.Vect())) / ptprobe;
        double mnf = 0 + (p4mn.Vect().Dot(p4f.Vect())) / ptprobe;
        double muf = 0 + (p4mu.Vect().Dot(p4f.Vect())) / ptprobe;
        // double mof = 0 + (p4mo.Vect().Dot(p4f.Vect()))/ptprobe;

        double m0fx = 1 + (p4m0.Vect().Dot(p4fx.Vect())) / ptprobe;
        double m2fx = 1 + (p4m2.Vect().Dot(p4fx.Vect())) / ptprobe;

        // Dijet mass
        p4dj = p4t;
        p4dj += p4p;
        double mjj = p4dj.M();
        double deta = fabs(p4p.Eta() - p4t.Eta());

        bool isdijet = (fabs(p4t.Eta()) < 1.3 && dphi > 2.7 &&
                        fabs(asymm) < maxa && //!
                        p4t.Pt() > 15. && Jet_jetId[itag] >= 4 &&
                        p4p.Pt() > 15. && Jet_jetId[iprobe] >= 4 &&
                        !Jet_jetveto[itag] && !Jet_jetveto[iprobe] && //!
                        pass_METfilter > 0);
        // DESY selection. Note tighter asymmetry cut and allJetsGood
        bool isdijet2 = (fabs(p4t.Eta()) < 1.3 && dphi > 2.7 &&
                         fabs((pttag - ptprobe) / (pttag + ptprobe)) < 0.7 && //!
                         // fabs(asymm)<maxa && //!
                         p4t.Pt() > 15. && Jet_jetId[itag] >= 4 &&
                         p4p.Pt() > 15. && Jet_jetId[iprobe] >= 4 &&
                         //! Jet_jetveto[itag] && !Jet_jetveto[iprobe] && //!
                         allJetsGood && //!
                         pass_METfilter > 0);

        for (int itrg = 0; itrg != ntrg; ++itrg)
        {

          if (debugevent)
            cout << "Check trigger #" << itrg << " for dijet "
                 << endl
                 << flush;

          string &trg = vtrg[itrg];
          if (!(*mtrg[trg]))
            continue;

// Plot without jet veto test (to be approved, Nestor)
	  if (doJetveto && fabs(p4t.Eta()) < 1.3 && dphi > 2.7 &&
                        fabs(asymm) < maxa && //!
                        p4t.Pt() > 15. && Jet_jetId[itag] >= 4 &&
                        p4p.Pt() > 15. && Jet_jetId[iprobe] >= 4 &&
                        //!Jet_jetveto[itag] && !Jet_jetveto[iprobe] && //!
                        pass_METfilter > 0)
	  //if (doJetveto && isdijet)
	  {
            jetvetoHistos *h = mhjv[trg];

            if (ptave >= h->ptmin && ptave < h->ptmax &&
                fabs(eta) >= h->absetamin && fabs(eta) < h->absetamax)
            {
              h->p2asymm_noveto->Fill(eta, p4p.Phi(), asymm, w);
	      h->h3asymm->Fill(eta, p4p.Phi(), asymm, w);
            }
	    // HF studies. Nestor. Aug 27, 2024.
	    if (doJetvetoVariants)
            {
              if (doPFComposition && fabs(p4t.Eta()) < 1.3)
              {
                if (pttag >= h->ptmin && pttag < h->ptmax)
                {
                  h->p2asymm_noveto_pttag->Fill(eta, p4p.Phi(), asymm, w);
		  h->p2chf_noveto_pttag->Fill(eta, p4p.Phi(), Jet_chHEF[iprobe], w);
                  h->p2nhf_noveto_pttag->Fill(eta, p4p.Phi(), Jet_neHEF[iprobe], w);
                  h->p2nef_noveto_pttag->Fill(eta, p4p.Phi(), Jet_neEmEF[iprobe], w);
                }
              }
            }
	    ////
	  }
// end of the test 
          if (doJetveto && isdijet)
          {
            jetvetoHistos *h = mhjv[trg];

            if (doJetvetoVariants)
              h->h2ptaeta_all->Fill(eta, ptave, w);
            if (ptave >= h->ptmin && ptave < h->ptmax &&
                fabs(eta) >= h->absetamin && fabs(eta) < h->absetamax)
            {
              if (doJetvetoVariants)
                h->h2ptaeta_sel->Fill(eta, ptave, w);
              h->p2asymm->Fill(eta, p4p.Phi(), asymm, w);
              h->h2phieta_ave->Fill(eta, p4p.Phi(), w);
            }

            if (doJetvetoVariants)
            {
              if (doPFComposition && fabs(p4t.Eta()) < 1.3)
              {
                h->h2pteta_all_pttag->Fill(eta, ptave, w);
                if (pttag >= h->ptmin && pttag < h->ptmax)
                { 
	          // HF studies. Nestor. Aug 27, 2024.
	          h->p2asymm_pttag->Fill(eta, p4p.Phi(), asymm, w);
		  //
                  h->h2pteta_sel_pttag->Fill(eta, pttag, w);
                  h->h2phieta_pttag->Fill(eta, p4.Phi(), w);
                  h->p2chf_pttag->Fill(eta, p4p.Phi(), Jet_chHEF[iprobe], w);
                  h->p2nhf_pttag->Fill(eta, p4p.Phi(), Jet_neHEF[iprobe], w);
                  h->p2nef_pttag->Fill(eta, p4p.Phi(), Jet_neEmEF[iprobe], w);
                }
              }
            }
          } // doJetveto

          if (doDijet && isdijet)
          {

            dijetHistos *h = mhdj[trg];
            double res = Jet_RES[iprobe] / Jet_RES[itag];



	    h->h2pteta_aball->Fill(eta, ptavp2, w);
            h->h2pteta_adall->Fill(eta, ptave, w);
            h->h2pteta_tcall->Fill(eta, pttag, w);
            h->h2pteta_pfall->Fill(eta, ptprobe, w);

            // Bisector (proper)
            if (ptavp2 >= h->ptmin && ptavp2 < h->ptmax &&
                fabs(eta) >= h->absetamin && fabs(eta) < h->absetamax)
            {
              h->h2pteta_absel->Fill(eta, ptavp2, w);
            }
            { // Bisector (proper)
              if (doDijetJER)
              {
                h->p2m0->Fill(eta, ptavp2, m0b, w);
                h->p2m0x->Fill(eta, ptavp2, m0bx, w);
                h->p2m2->Fill(eta, ptavp2, m2b, w);
                h->p2m2x->Fill(eta, ptavp2, m2bx, w);
              }
              if (doPFComposition)
              {
                h->p2pt->Fill(eta, ptavp2, Jet_pt[iprobe], w);
                h->p2rho->Fill(eta, ptavp2, rho, w);
                h->p2chf->Fill(eta, ptavp2, Jet_chHEF[iprobe], w);
                h->p2nhf->Fill(eta, ptavp2, Jet_neHEF[iprobe], w);
                h->p2nef->Fill(eta, ptavp2, Jet_neEmEF[iprobe], w);
                h->p2cef->Fill(eta, ptavp2, Jet_chEmEF[iprobe], w);
                h->p2muf->Fill(eta, ptavp2, Jet_muEF[iprobe], w);

                h->ppt13->Fill(ptavp2, Jet_pt[itag], w);
                h->prho13->Fill(ptavp2, rho, w);
                h->pchf13->Fill(ptavp2, Jet_chHEF[itag], w);
                h->pnhf13->Fill(ptavp2, Jet_neHEF[itag], w);
                h->pnef13->Fill(ptavp2, Jet_neEmEF[itag], w);
                h->pcef13->Fill(ptavp2, Jet_chEmEF[itag], w);
                h->pmuf13->Fill(ptavp2, Jet_muEF[itag], w);
              }

              h->p2resab->Fill(eta, ptavp2, res, w);
              h->p2m0ab->Fill(eta, ptavp2, m0b, w);
              h->p2m2ab->Fill(eta, ptavp2, m2b, w);
              h->p2mnab->Fill(eta, ptavp2, mnb, w);
              h->p2muab->Fill(eta, ptavp2, mub, w);
            }
            // Dijet axis
            if (ptave >= h->ptmin && ptave < h->ptmax &&
                fabs(eta) >= h->absetamin && fabs(eta) < h->absetamax)
            {
              h->h2pteta_adsel->Fill(eta, ptave, w);
            }
            { // Dijet axis
              h->p2resad->Fill(eta, ptave, res, w);
              h->p2m0ad->Fill(eta, ptave, m0d, w);
              h->p2m2ad->Fill(eta, ptave, m2d, w);
              h->p2mnad->Fill(eta, ptave, mnd, w);
              h->p2muad->Fill(eta, ptave, mud, w);
            }
            // Tag jet axis
            if (pttag >= h->ptmin && pttag < h->ptmax)
            {
              h->h2pteta_tcsel->Fill(eta, pttag, w);
            }
            // Tag jet axis
            {
              h->p2restc->Fill(eta, pttag, res, w);
              h->p2m0tc->Fill(eta, pttag, m0c, w);
              h->p2m2tc->Fill(eta, pttag, m2c, w);
              h->p2mntc->Fill(eta, pttag, mnc, w);
              h->p2mutc->Fill(eta, pttag, muc, w);
            }
            // Probe jet axis
            if (ptprobe >= h->ptmin && ptprobe < h->ptmax)
            {
              h->h2pteta_pfsel->Fill(eta, ptprobe, w);
            }
            // Probe jet axis
            {
              h->p2respf->Fill(eta, ptprobe, res, w);
              h->p2m0pf->Fill(eta, ptprobe, m0f, w);
              h->p2m2pf->Fill(eta, ptprobe, m2f, w);
              h->p2mnpf->Fill(eta, ptprobe, mnf, w);
              h->p2mupf->Fill(eta, ptprobe, muf, w);
            }
          } // doDijet

          if (doGluonJets && isdijet)
          {
            /*
            gluonHistos *h = mhgj[trg];
            double res = Jet_RES[iprobe] / Jet_RES[itag];
	    for (int i = 0; i != njet; ++i){
	      if (Jet_btagPNetQvG[i] < WP_t){
	        //std::cout << "We pass the WP that is: " << Jet_btagPNetQvG[i] << std::endl;
                { // Bisector (proper) WP_t
                  if (doDijetJER)
		  {
                    h->p2m0_t->Fill(eta, ptavp2, m0b, w);
                    h->p2m0x_t->Fill(eta, ptavp2, m0bx, w);
                    h->p2m2_t->Fill(eta, ptavp2, m2b, w);
                    h->p2m2x_t->Fill(eta, ptavp2, m2bx, w);
                  }

                  h->p2m0ab_t->Fill(eta, ptavp2, m0b, w);
                  h->p2m2ab_t->Fill(eta, ptavp2, m2b, w);
                  h->p2mnab_t->Fill(eta, ptavp2, mnb, w);
                  h->p2muab_t->Fill(eta, ptavp2, mub, w);
                }
                { // Dijet axis
                  h->p2m0ad_t->Fill(eta, ptave, m0d, w);
                  h->p2m2ad_t->Fill(eta, ptave, m2d, w);
                  h->p2mnad_t->Fill(eta, ptave, mnd, w);
                  h->p2muad_t->Fill(eta, ptave, mud, w);
                }
                // Tag jet axis
                {
                  h->p2m0tc_t->Fill(eta, pttag, m0c, w);
                  h->p2m2tc_t->Fill(eta, pttag, m2c, w);
                  h->p2mntc_t->Fill(eta, pttag, mnc, w);
                  h->p2mutc_t->Fill(eta, pttag, muc, w);
                }
                // Probe jet axis
                {
                 h->p2m0pf_t->Fill(eta, ptprobe, m0f, w);
                 h->p2m2pf_t->Fill(eta, ptprobe, m2f, w);
                 h->p2mnpf_t->Fill(eta, ptprobe, mnf, w);
                 h->p2mupf_t->Fill(eta, ptprobe, muf, w);
                }
	      } //WP_t

              if (Jet_btagPNetQvG[i] < WP_m){
                //std::cout << "We pass the WP that is: " << Jet_btagPNetQvG[i] << std::endl;
                { // Bisector (proper) WP_t
                  if (doDijetJER)
                  {
                    h->p2m0_m->Fill(eta, ptavp2, m0b, w);
                    h->p2m0x_m->Fill(eta, ptavp2, m0bx, w);
                    h->p2m2_m->Fill(eta, ptavp2, m2b, w);
                    h->p2m2x_m->Fill(eta, ptavp2, m2bx, w);
                  }

                  h->p2m0ab_m->Fill(eta, ptavp2, m0b, w);
                  h->p2m2ab_m->Fill(eta, ptavp2, m2b, w);
                  h->p2mnab_m->Fill(eta, ptavp2, mnb, w);
                  h->p2muab_m->Fill(eta, ptavp2, mub, w);
                }
                { // Dijet axis
                  h->p2m0ad_m->Fill(eta, ptave, m0d, w);
                  h->p2m2ad_m->Fill(eta, ptave, m2d, w);
                  h->p2mnad_m->Fill(eta, ptave, mnd, w);
                  h->p2muad_m->Fill(eta, ptave, mud, w);
                }
                // Tag jet axis
                {
                  h->p2m0tc_m->Fill(eta, pttag, m0c, w);
                  h->p2m2tc_m->Fill(eta, pttag, m2c, w);
                  h->p2mntc_m->Fill(eta, pttag, mnc, w);
                  h->p2mutc_m->Fill(eta, pttag, muc, w);
                }
                // Probe jet axis
                {
                 h->p2m0pf_m->Fill(eta, ptprobe, m0f, w);
                 h->p2m2pf_m->Fill(eta, ptprobe, m2f, w);
                 h->p2mnpf_m->Fill(eta, ptprobe, mnf, w);
                 h->p2mupf_m->Fill(eta, ptprobe, muf, w);
                }
              } // WP_m

              if (Jet_btagPNetQvG[i] < WP_l){
                //std::cout << "We pass the WP that is: " << Jet_btagPNetQvG[i] << std::endl;
                { // Bisector (proper) WP_t
                  if (doDijetJER)
                  {
                    h->p2m0_l->Fill(eta, ptavp2, m0b, w);
                    h->p2m0x_l->Fill(eta, ptavp2, m0bx, w);
                    h->p2m2_l->Fill(eta, ptavp2, m2b, w);
                    h->p2m2x_l->Fill(eta, ptavp2, m2bx, w);
                  }

                  h->p2m0ab_l->Fill(eta, ptavp2, m0b, w);
                  h->p2m2ab_l->Fill(eta, ptavp2, m2b, w);
                  h->p2mnab_l->Fill(eta, ptavp2, mnb, w);
                  h->p2muab_l->Fill(eta, ptavp2, mub, w);
                }
                { // Dijet axis
                  h->p2m0ad_l->Fill(eta, ptave, m0d, w);
                  h->p2m2ad_l->Fill(eta, ptave, m2d, w);
                  h->p2mnad_l->Fill(eta, ptave, mnd, w);
                  h->p2muad_l->Fill(eta, ptave, mud, w);
                }
                // Tag jet axis
                {
                  h->p2m0tc_l->Fill(eta, pttag, m0c, w);
                  h->p2m2tc_l->Fill(eta, pttag, m2c, w);
                  h->p2mntc_l->Fill(eta, pttag, mnc, w);
                  h->p2mutc_l->Fill(eta, pttag, muc, w);
                }
                // Probe jet axis
                {
                 h->p2m0pf_l->Fill(eta, ptprobe, m0f, w);
                 h->p2m2pf_l->Fill(eta, ptprobe, m2f, w);
                 h->p2mnpf_l->Fill(eta, ptprobe, mnf, w);
                 h->p2mupf_l->Fill(eta, ptprobe, muf, w);
                }
              } // WP_l

	    } //njets 
          */
          } // doGluonJets
          


          if (doDijet2 && isdijet2)
          {

            dijetHistos2 *h = mhdj2[trg];
            double res = Jet_RES[iprobe] / Jet_RES[itag];
            double jsf = (smearJets && Jet_CF[itag] > 0 ? Jet_CF[iprobe] / Jet_CF[itag] : 1);

            double abseta = fabs(eta);
            h->h2pteta->Fill(abseta, ptavp2, w);

            h->p2res->Fill(abseta, ptavp2, res, w);
            h->p2jsf->Fill(abseta, ptavp2, jsf, w);
            h->p2m0->Fill(abseta, ptavp2, m0b, w);
            h->p2m2->Fill(abseta, ptavp2, m2b, w);
            h->p2mn->Fill(abseta, ptavp2, mnb, w);
            h->p2mu->Fill(abseta, ptavp2, mub, w);

            h->p2m0x->Fill(abseta, ptavp2, m0bx, w);
            h->p2m2x->Fill(abseta, ptavp2, m2bx, w);

	    // JER studies
	    h->h3m0->Fill(abseta, ptavp2, m0b, w);
            h->h3m2->Fill(abseta, ptavp2, m2b, w);

            // Extras for FSR studies
            h->p2mnu->Fill(abseta, ptavp2, mnub, w);
            h->p2mnx->Fill(abseta, ptavp2, mnbx, w);
            h->p2mux->Fill(abseta, ptavp2, mubx, w);
            h->p2mnux->Fill(abseta, ptavp2, mnubx, w);

            h->h2ptetatc->Fill(abseta, pttag, w);
            h->p2restc->Fill(abseta, pttag, res, w);
            h->p2jsftc->Fill(abseta, pttag, jsf, w);
            h->p2m0tc->Fill(abseta, pttag, m0c, w);
            h->p2m2tc->Fill(abseta, pttag, m2c, w);
            h->p2mntc->Fill(abseta, pttag, mnc, w);
            h->p2mutc->Fill(abseta, pttag, muc, w);

            h->h2ptetapf->Fill(abseta, ptprobe, w);
            h->p2respf->Fill(abseta, ptprobe, res, w);
            h->p2jsfpf->Fill(abseta, ptprobe, jsf, w);
            h->p2m0pf->Fill(abseta, ptprobe, m0f, w);
            h->p2m2pf->Fill(abseta, ptprobe, m2f, w);
            h->p2mnpf->Fill(abseta, ptprobe, mnf, w);
            h->p2mupf->Fill(abseta, ptprobe, muf, w);
          } // doDijet2

        } // for itrg

        // Dijet without deltaphi cut
        if (fabs(p4t.Eta() < 1.3) && fabs(asymm) < maxa)
        {

          if (ptave >= 40)
          {
            h2dphi->Fill(p4p.Eta(), dphi, w);
          }
        }
      } // for itag
    }   // njet>=2

    // Multijet selection
    if (ismultijet && doMultijet)
    {

      if (debugevent)
        cout << "Analyze multijet" << endl
             << flush;

      // pTave binning
      double ptlead = p4lead.Pt();
      double ptrecoil = p4recoil.Pt();
      double ptave = 0.5 * (ptlead + ptrecoil);
      // double ptavp3 defined earlier, as is p4b3
      if (doPU_per_trigger){
         get_weight(ptlead, 0, "doMultijets");
      }
      //get_weight(ptavp3, 0, "doMultijets");

      //get_weight(ptrecoil, 0, "doMultijets");

      //get_weight(ptave, 0, "doMultijets");
        //std::cerr << "w weight before the func: " << w << std::endl;  
        //std::cerr << "w_ptlead before the func: " << w_ptlead << std::endl;
        //std::cerr << "w_ptavp3 before the func: " << w_ptavp3 << std::endl;
	//std::cerr << "w_ptave before the func: " << w_ptave << std::endl;
        //std::cerr << "w_ptrecoil before the func: " << w_ptrecoil << std::endl;


        if (doPU_per_trigger){
           //get_weight(ptlead, p4l.Eta(), "doMultijets");
           //get_weight(700., 2., "doMultijets");
           //std::cerr << "ptlead: " << ptlead << " and eta are " << p4l.Eta() <<std::endl;
           //std::cerr << "PU_weight: " << PU_weight <<std::endl;
           if (PU_weight > 0){
              w_ptlead = w * PU_weight;
              w_ptavp3 = w * PU_weight;
	      w_ptave = w * PU_weight;
              w_ptrecoil = w * PU_weight;
           }
           else { // if cero, should be = 1? or = w? 
              w_ptlead = w;
              w_ptavp3 = w;
	      w_ptave = w;
              w_ptrecoil = w;
           }
        }
        else {
           w_ptlead = w;
           w_ptavp3 = w;
	   w_ptave = w;
           w_ptrecoil = w;
        }
        //std::cerr << "w weight after the func: " << w << std::endl;
        //std::cerr << "w_ptlead: " << w_ptlead << std::endl;
        //std::cerr << "w_ptavp3: " << w_ptavp3 << std::endl;
	//std::cerr << "w_ptave: " << w_ptave << std::endl;
        //std::cerr << "w_ptrecoil: " << w_ptrecoil << std::endl;
	


      //std::cerr << "PU_weight: " << PU_weight <<std::endl;
      //

      // Projection to transverse plane (is this necessary?)
      p4m0.SetPtEtaPhiM(p4m0.Pt(), 0., p4m0.Phi(), 0.);
      p4m3.SetPtEtaPhiM(p4m3.Pt(), 0., p4m3.Phi(), 0.);
      p4mn3.SetPtEtaPhiM(p4mn3.Pt(), 0., p4mn3.Phi(), 0.);

      // Bisector axis p4b3 defined earlier (equal angles)
      double m0b = 1 + (p4m0.Vect().Dot(p4b3.Vect())) / ptave;
      double m3b = 1 + (p4m3.Vect().Dot(p4b3.Vect())) / ptave;
      double mnb = 0 + (p4mn3.Vect().Dot(p4b3.Vect())) / ptave;
      double mub = 0 + (p4mu3.Vect().Dot(p4b3.Vect())) / ptave;

      // Dijet axis (not equal angles)
      p4m.SetPtEtaPhiM(0, 0, 0, 0);
      p4m -= p4lead;
      p4m += p4recoil;
      p4m.SetPtEtaPhiM(p4m.Pt(), 0., p4m.Phi(), 0.);
      p4m *= 1. / p4m.Pt();

      double m0m = 1 + (p4m0.Vect().Dot(p4m.Vect())) / ptave;
      double m3m = 1 + (p4m3.Vect().Dot(p4m.Vect())) / ptave;
      double mnm = 0 + (p4mn3.Vect().Dot(p4m.Vect())) / ptave;
      double mum = 0 + (p4mu3.Vect().Dot(p4m.Vect())) / ptave;

      p4l.SetPtEtaPhiM(0, 0, 0, 0);
      p4l -= p4lead;
      p4l.SetPtEtaPhiM(p4l.Pt(), 0., p4l.Phi(), 0.);
      p4l *= 1. / p4l.Pt();

      double m0l = 1 + (p4m0.Vect().Dot(p4l.Vect())) / ptlead;
      double m3l = 1 + (p4m3.Vect().Dot(p4l.Vect())) / ptlead;
      double mnl = 0 + (p4mn3.Vect().Dot(p4l.Vect())) / ptlead;
      double mul = 0 + (p4mu3.Vect().Dot(p4l.Vect())) / ptlead;

      p4r.SetPtEtaPhiM(0, 0, 0, 0);
      p4r += p4recoil;
      p4r.SetPtEtaPhiM(p4r.Pt(), 0., p4r.Phi(), 0.);
      p4r *= 1. / p4r.Pt();

      double m0r = 1 + (p4m0.Vect().Dot(p4r.Vect())) / ptrecoil;
      double m3r = 1 + (p4m3.Vect().Dot(p4r.Vect())) / ptrecoil;
      double mnr = 0 + (p4mn3.Vect().Dot(p4r.Vect())) / ptrecoil;
      double mur = 0 + (p4mu3.Vect().Dot(p4r.Vect())) / ptrecoil;

      for (int itrg = 0; itrg != ntrg; ++itrg)
      {

        string &trg = vtrg[itrg];
        if (!(*mtrg[trg]))
          continue;

        multijetHistos *h = mhmj[trg];


	// PU reweighintg histograms
	// Histograms from multijet analysis
	// Nestor. Sep 12, 2024.
        h->pm0l->Fill(ptlead, m0l, w_ptlead);
        h->pm0a->Fill(ptavp3, m0b, w_ptavp3);
	h->pm0r->Fill(ptrecoil, m0r, w_ptrecoil);
        h->pm2l->Fill(ptlead, m3l, w_ptlead);
        h->pm2a->Fill(ptavp3, m3b, w_ptavp3);
        h->pm2r->Fill(ptrecoil, m3r, w_ptrecoil);
        //

        h->hpta_all->Fill(ptavp3, w);
        h->hptm_all->Fill(ptave, w);
        h->hptl_all->Fill(ptlead, w);
        h->hptr_all->Fill(ptrecoil, w);

        if (ptavp3 >= h->ptmin && ptavp3 < h->ptmax)
          h->hpta_sel->Fill(ptavp3, w);
        if (ptave >= h->ptmin && ptave < h->ptmax)
          h->hptm_sel->Fill(ptave, w);
        if (ptlead >= h->ptmin && ptlead < h->ptmax)
          h->hptl_sel->Fill(ptlead, w);
        if (ptrecoil >= h->ptmin && ptrecoil < h->ptmax)
          h->hptr_sel->Fill(ptrecoil, w);

        double res = (p4leadRES.Pt() / p4recoilRES.Pt()) /
                     (p4lead.Pt() / p4recoil.Pt());


        h->presa->Fill(ptavp3, res, w);
        h->presm->Fill(ptave, res, w);
        h->presl->Fill(ptlead, res, w);
        h->presr->Fill(ptrecoil, res, w);

        h->ptleada->Fill(ptavp3, ptlead, w);
        h->ptleadm->Fill(ptave, ptlead, w);
        h->ptleadl->Fill(ptlead, ptlead, w);
        h->ptleadr->Fill(ptrecoil, ptlead, w);

        h->pcrecoila->Fill(ptavp3, Crecoil, w);
        h->pcrecoilm->Fill(ptave, Crecoil, w);
        h->pcrecoill->Fill(ptlead, Crecoil, w);
        h->pcrecoilr->Fill(ptrecoil, Crecoil, w);

        //h->pm0a->Fill(ptavp3, m0b, w);
        //h->pm2a->Fill(ptavp3, m3b, w);
        h->pmna->Fill(ptavp3, mnb, w);
        h->pmua->Fill(ptavp3, mub, w);

        h->pm0m->Fill(ptave, m0m, w);
        h->pm2m->Fill(ptave, m3m, w);
        h->pmnm->Fill(ptave, mnm, w);
        h->pmum->Fill(ptave, mum, w);

        //h->pm0l->Fill(ptlead, m0l, w);
        //h->pm2l->Fill(ptlead, m3l, w);
        h->pmnl->Fill(ptlead, mnl, w);
        h->pmul->Fill(ptlead, mul, w);

        //h->pm0r->Fill(ptrecoil, m0r, w);
        //h->pm2r->Fill(ptrecoil, m3r, w);
        h->pmnr->Fill(ptrecoil, mnr, w);
        h->pmur->Fill(ptrecoil, mur, w);

        if (doMultijetControl)
        {
          h->h2m0a->Fill(ptavp3, m0b, w);
          h->h2m2a->Fill(ptavp3, m3b, w);
          if (ptave > 1.25 * h->trgpt)
            h->hcosdphi->Fill(cos(DELTAPHI(p4lead.Phi(), p4recoil.Phi())), w);
        }

        if (doPFComposition)
        {
          h->ppt13->Fill(ptavp3, Jet_pt[0], w);
          h->prho13->Fill(ptavp3, rho, w);
          h->pchf13->Fill(ptavp3, Jet_chHEF[0], w);
          h->pnhf13->Fill(ptavp3, Jet_neHEF[0], w);
          h->pnef13->Fill(ptavp3, Jet_neEmEF[0], w);
          h->pcef13->Fill(ptavp3, Jet_chEmEF[0], w);
          h->pmuf13->Fill(ptavp3, Jet_muEF[0], w);
        } // doPFcomposition

      } // for itrg
    }   // ismultijet

    //doJetsperRun
    if (doJetsperRuns && dolumi)
    {
      auto it5 = std::find(_runNumberBin.begin(), _runNumberBin.end(), run);
      double ptlead = p4lead.Pt();
      p4l.SetPtEtaPhiM(0, 0, 0, 0);
      p4l -= p4lead;
      p4l.SetPtEtaPhiM(p4l.Pt(), 0., p4l.Phi(), 0.);
      p4m0.SetPtEtaPhiM(p4m0.Pt(), 0., p4m0.Phi(), 0.);
      p4m3.SetPtEtaPhiM(p4m3.Pt(), 0., p4m3.Phi(), 0.);
      p4l *= 1. / p4l.Pt();
      double m0l = 1 + (p4m0.Vect().Dot(p4l.Vect())) / ptlead; // MPF leading
      double m3l = 1 + (p4m3.Vect().Dot(p4l.Vect())) / ptlead; // DB leading
      for (int itrg = 0; itrg != ntrg; ++itrg)
      {
        string &trg = vtrg[itrg];
        //std::cout << "Trigger: " << trg << std::endl;
        if (!(*mtrg[trg]))
          continue;
        jetsperRuns *h = mjet[trg];
        //const range &r = mt[trg];
	
	//HLT_PFJet500
        if (ismultijet && itrg == 9 && p4lead.Pt() >= h->ptmin && p4lead.Pt() < h->ptmax &&
            fabs(p4lead.Eta()) > h->absetamin &&
            fabs(p4lead.Eta()) < h->absetamax &&
	    it5 != _runNumberBin.end() && mlumi[trg][run] > 0 ) //Jet_jetId[itag] >= 4
	{
          if (ptlead > 500){
            h->pMPF_500->Fill(run, m0l, 1./mlumi[trg][run]);
            h->pDB_500->Fill(run, m3l, 1./mlumi[trg][run]);
          }
          if (ptlead > 600){
            h->pMPF_600->Fill(run, m0l, 1./mlumi[trg][run]);
            h->pDB_600->Fill(run, m3l, 1./mlumi[trg][run]);
          }
          if (ptlead > 800){
            h->pMPF_800->Fill(run, m0l, 1./mlumi[trg][run]);
            h->pDB_800->Fill(run, m3l, 1./mlumi[trg][run]);
          }
          if (ptlead > 1000){
            h->pMPF_1000->Fill(run, m0l, 1./mlumi[trg][run]);
            h->pDB_1000->Fill(run, m3l, 1./mlumi[trg][run]);
          }
          if (ptlead > 1200){
            h->pMPF_1200->Fill(run, m0l, 1./mlumi[trg][run]);
            h->pDB_1200->Fill(run, m3l, 1./mlumi[trg][run]);
          }
          // Barrel region	  
	  if (p4lead.Eta() < 1.3)
	  {
	    if (ptlead > 500){
              h->pMPF_500b->Fill(run, m0l, 1./mlumi[trg][run]);
              h->pDB_500b->Fill(run, m3l, 1./mlumi[trg][run]);
            }
            if (ptlead > 600){
              h->pMPF_600b->Fill(run, m0l, 1./mlumi[trg][run]);
              h->pDB_600b->Fill(run, m3l, 1./mlumi[trg][run]);
            }
            if (ptlead > 800){
              h->pMPF_800b->Fill(run, m0l, 1./mlumi[trg][run]);
              h->pDB_800b->Fill(run, m3l, 1./mlumi[trg][run]);
            }
            if (ptlead > 1000){
              h->pMPF_1000b->Fill(run, m0l, 1./mlumi[trg][run]);
              h->pDB_1000b->Fill(run, m3l, 1./mlumi[trg][run]);
            }
            if (ptlead > 1200){
              h->pMPF_1200b->Fill(run, m0l, 1./mlumi[trg][run]);
              h->pDB_1200b->Fill(run, m3l, 1./mlumi[trg][run]);
            }
	  }
	}
	for (int i = 0; i != njet; ++i)
        {
          p4.SetPtEtaPhiM(Jet_pt[i], Jet_eta[i], Jet_phi[i], Jet_mass[i]);
          if (Jet_jetId[i] >= 4 && !Jet_jetveto[i] && pass_METfilter > 0 ) //&& fabs(p4.Eta()) < 1.3)  
          {

            w = (isMC ? genWeight : 1.);
            h->h1jetrate->Fill(run, w);
            if (it5 != _runNumberBin.end() && mlumi[trg][run] > 0){
              //std::cout << run << " is included in runNumberBin and the rec luminosity is: " << _lums[run] << std::endl;
              w = (isMC ? genWeight : 1./mlumi[trg][run]);
            }
            else {
              //w = (isMC ? genWeight : 1.);
	      //std::cout << run << " is not included in runNumberBin and the rec luminosity is: " << _lums[run] << std::endl;
              //exit(0);
              continue;
            }
            if (p4.Pt() >= h->ptmin && p4.Pt() < h->ptmax &&
                fabs(p4.Rapidity()) > h->absetamin &&
                fabs(p4.Rapidity()) < h->absetamax) {
                  //std::cout << "The p4.pt is: " << p4.Pt() << " and the pt min is: " << h->ptmin << " and the pt max is: " << h->ptmax << " from the trg: " << itrg << " weight: " << w << " and the itrg is: " << itrg << std::endl;
                h->h1jetxsec->Fill(run, w);
                h->h2jetpteta->Fill(fabs(p4.Eta()), p4.Pt(), w);
		//h->p2MPF->Fill(run, ptlead, m0l, w);
		//std::cout << "The ptlead is: " << ptlead << " , the run is: " << run << " and the m0l: " << m0l << std::endl;
                if (fabs(p4.Eta()) < 1.3) {
                  h->h1pt13_w->Fill(p4.Pt(), w);
                  h->h1pt13->Fill(p4.Pt(), 1.);
		  //h->p2MPF_bar->Fill(run, ptlead, m0l, w);
                }
            } //else {
	      //std::cout << "It is not passing the pt and eta requirements " << std::endl;
	    //}
          } //else {
	    //std::cout << "It is not passing the jet ID, the jet veto and the filters requirements " << std::endl;
	    //std::cout << "Jet ID is: " << Jet_jetId[i] << " Jet veto: " << !Jet_jetveto[i] << " and pass Filters" << pass_METfilter << std::endl;
	    //if (Jet_jetId[i] >= 4) {
	      //std::cout << "Jet ID is bigger or equal than 4" << std::endl;
	    //} else {
	      //std::cout << "Jet ID less than 4" << std::endl;
	    //}
	  //}
        }
      }
      w = (isMC ? genWeight : 1.);
    } // doJetsperRun



    // Lumi
    /*
    if (dolumi && mrunls[run][luminosityBlock] == 0)
    {
      // DOES THIS WORK FOR ALL THE TRIGGERS?
      // WHAT IS THE INFORMATION WE'RE INTERESTED IN?
      for (int itrg = 0; itrg != ntrg; ++itrg)
      {

        string &trg = vtrg[itrg];
        if (!(*mtrg[trg]))
          continue;

        lumiHistos *h = mhlumi[trg];
        float trpu = _avgpu[run][luminosityBlock];

        if (debugevent)
        {
          cout << "Analyze lumi" << endl
               << flush;
        }
        //float lum = _lums[run][luminosityBlock];
        //float lum2 = _lums2[run][luminosityBlock];
        float lum = _lums[run];
        //float lum2 = _lums2[run][luminosityBlock];
        h->lum += lum; // /prescale
        //h->lum2 += lum2; // /prescale
        // TODO:
        // PRESCALE INFORMATION
        
        double prescale(0);
        auto ip = _prescales[h->trigname].find(run);
        if (ip==_prescales[h->trigname].end()) {
          PrintInfo(Form("No prescale info for trigger %s in run %d!",h->trigname.c_str(),run));
          assert(false);
        } else {
          prescale = ip->second;
          if (prescale==0) {
            PrintInfo(Form("Prescale zero for trigger %s in run %d!",h->trigname.c_str(),run));
            prescale = 1.;
            assert(false);
          }
        }
        
        h->htrglumi->Fill(lum, w); //  / prescale
        h->htrgpu->Fill(trpu, w);
	//h->hLumiPerRun->Fill(run, lum, w);
        //h->hnpv->Fill(PV_npvs, w);
        h->hnpvgood->Fill(PV_npvsGood, w);

        h_trgvslumi->Fill(itrg, lum, w);
        h_trgvspu->Fill(itrg, trpu, w);
      }
      mrunls[run][luminosityBlock] = 1;
    } // doLumi
    */

    h2mhtvsmet->Fill(p4t1met.Pt(), p4mht.Pt(), w);
  } // for jentry
  cout << endl
       << flush;

  cout << "Finished looping over " << nevt << " of which " << _ngoodevts
       << " passed trigger. Start writing file." << endl
       << flush;
  //cout << "CCCC :"  <<htrg->GetNbinsX() << endl;
  fout->Write();
  fout->Close();
  cout << "File written and closed." << endl
       << flush;

  if (doJSON)
    cout << Form("Found %d bad events according to new JSON (events cut)", _nbadevts_json) << endl;
  if (doTrigger)
  {
    cout << Form("Found %d bad events according to trigger bits (events cut)", _nbadevts_trg) << endl;
    cout << Form("Found %d bad events not in fwd trigger phase space (events cut)", _nbadevts_fwdtrg) << endl;
  }

  cout << "Processed " << nrun << " runs, "
       << nls << " luminosity blocks and " << nevt << " events" << endl;
  cout << "Saving these to file rootfiles/jmenano.json for brilcalc" << endl;

  ofstream fjson(Form("rootfiles/%s/jmenano_%s_%s.json",
                      version.c_str(),
                      dataset.c_str(), version.c_str()));
  fjson << "{" << endl;
  for (map<int, map<int, int>>::iterator it = mrunls.begin();
       it != mrunls.end(); ++it)
  {

    int run = it->first;
    int ls_prev(0), ls(0), ok(0);
    bool firstblock(true), newblock(true);

    if (it != mrunls.begin())
      fjson << "," << endl;
    fjson << "  \"" << run << "\": [";
    for (map<int, int>::iterator jt = it->second.begin();
         jt != it->second.end(); ++jt)
    {

      ok = jt->second;
      if (ok)
      {
        ls_prev = ls;
        ls = jt->first;
      }
      else
        continue;

      newblock = (firstblock || (ls - ls_prev > 1));
      if (newblock && ok)
      {
        if (firstblock)
        {
          fjson << "[" << ls << ", ";
          firstblock = false;
        }
        else
        {
          fjson << ls_prev << "], [" << ls << ", ";
        }
      }
    } // for jt
    fjson << ls << "]]";
  } // for it
  fjson << "}" << endl;

  cout << Form("Analyzed %d events", _ngoodevts) << endl;
  cout << "Saving these to " << fout->GetName() << " for drawJMENANO.C" << endl;



  // h2mhtvsmet->Draw("COLZ");
} // Loop()

// Code originally from jetphys/HistosFill.C
void DijetHistosFill::PrintInfo(string info, bool printcout)
{
  //*ferr << info << endl << flush;
  if (printcout)
    cout << info << endl
         << flush;
}
/*
// Load luminosity information
// TODO:
// Currently no good runs are defined
bool DijetHistosFill::LoadLumi()
{
  // For calculating luminosity on the fly based on .csv file and take only events with non-zero luminosity

  const char *lumifile = getLumifile(dataset.c_str());

  PrintInfo(string("Processing LoadLumi() with ") + lumifile + "...", true);

  // Check lumi against the list of good runs
  const int a_goodruns[] = {};
  const int ngoodruns = sizeof(a_goodruns) / sizeof(a_goodruns[0]);
  set<int> goodruns;
  if (ngoodruns > 0)
  { // This is an old remnant
    for (int runidx = 0; runidx != ngoodruns; ++runidx)
      goodruns.insert(a_goodruns[runidx]);

    for (auto runit = goodruns.begin(); runit != goodruns.end(); ++runit)
      cout << *runit << ", ";
    cout << endl;
  }
  set<pair<int, int>> nolums;

  ifstream f(lumifile, ios::in);
  if (!f.is_open())
    return false;
  float secLS = 2.3310e+01;
  string s;
  int rn, fill, ls, ifoo;
  float del, rec, avgpu, energy;
  char sfoo[512];
  bool getsuccess1 = static_cast<bool>(getline(f, s, '\n'));
  if (!getsuccess1)
    return false;
  PrintInfo(string("\nstring: ") + s + " !", true);

  // HOX: the lumi file format has been changing. Change the conditions when needed.
  if (s != "#Data tag : 23v1 , Norm tag: None")
    return false;

  bool getsuccess2 = static_cast<bool>(getline(f, s, '\n'));
  if (!getsuccess2)
    return false;
  PrintInfo(string("\nstring: ") + s + " !", true);
  if (s != "#run:fill,ls,time,beamstatus,E(GeV),delivered(/ub),recorded(/ub),avgpu,source")
    return false;

  int nls(0);
  double lumsum(0);
  double lumsum_good(0);
  double lumsum_json(0);
  bool skip(false);
  while (getline(f, s, '\n'))
  {
    // Skip if not STABLE BEAMS or wrong number of arguments
    // STABLE BEAMS alts: ADJUST, BEAM DUMP, FLAT TOP, INJECTION PHYSICS BEAM, N/A, RAMP DOWN, SETUP, SQUEEZE
    if (sscanf(s.c_str(), "%d:%d,%d:%d,%d/%d/%d %d:%d:%d,STABLE BEAMS,%f,%f,%f,%f,%s",
               &rn, &fill, &ls, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &ifoo, &energy, &del, &rec, &avgpu, sfoo) != 15)
      skip = true;

    if (debugevent)
      PrintInfo(Form("Run %d ls %d lumi %f/pb", rn, ls, rec * 1e-6), true);

    if (skip)
    { // The user should know if this happens, since we can choose to use only STABLE BEAMS
      if (skip)
        PrintInfo(string("Skipping line (effects the recorded lumi):\n") + s, true);
      skip = false;
      continue;
    }

    if (_lums[rn][ls] != 0)
      return false;
    if (_avgpu[rn][ls] != 0)
      return false;
    // lumiCalc.py returns lumi in units of mub-1 (=>nb-1=>pb-1)
    double lum = rec * 1e-6;
    double lum2 = del * 1e-6;
    if (lum == 0 and goodruns.find(rn) != goodruns.end() and (_json[rn][ls] == 1)) // The second condition had !jp::dojson or
      nolums.insert(pair<int, int>(rn, ls));

    _avgpu[rn][ls] = avgpu * 69000. / 80000.; // brilcalc --minBiasXsec patch
    _lums[rn][ls] = lum;
    _lums2[rn][ls] = lum2;
    lumsum += lum;
    if (goodruns.find(rn) != goodruns.end()) // Apr 17
      lumsum_good += lum;
    if ((_json[rn][ls]))
      lumsum_json += lum;
    ++nls;
    if (nls > 100000000)
      return false;
  }

  PrintInfo(Form("Called LoadLumi() with %s:\nLoaded %lu runs with %d lumi sections containing %f"
                 " pb-1 of data,\n of which %f pb-1 is in good runs (%f%%)\nThis corresponds to %f"
                 " hours of data-taking\nThe JSON file contains %f pb-1 (%f%%)",
                 lumifile, _lums.size(), nls, lumsum, lumsum_good,
                 100. * lumsum_good / lumsum, nls * secLS / 3600, lumsum_json, 100. * lumsum_json / lumsum),
            true);

  // Report any empty lumi section
  if (nolums.size() != 0)
  {
    PrintInfo(Form("Warning, found %lu non-normalizable LS:", nolums.size()), true);
    for (auto lumit = nolums.begin(); lumit != nolums.end(); ++lumit)
    {
      cout << " [" << lumit->first << "," << lumit->second;
      auto lumit2 = lumit;
      ++lumit2;
      if (lumit2->first != lumit->first or lumit2->second != lumit->second + 1)
        cout << "]";
      else
      {
        for (int lumadd = 0; lumit2 != nolums.end() and lumit2->first == lumit->first and
                             lumit2->second == lumit->second + lumadd + 1;
             ++lumadd, ++lumit2)
        {
        };
        lumit = --lumit2;
        cout << "-" << lumit->second << "]";
      }
    } // for lumit
    cout << endl;
  } // nolums

  cout << endl;
  return true;
} // LoadLumi

*/

// Code originally from jetphys/HistosFill.C
bool DijetHistosFill::LoadJSON(string json)
{
  PrintInfo(string("Processing LoadJSON() with ") + json + " ...", true);
  ifstream file(json, ios::in);
  if (!file.is_open())
  {
    assert(false);
    return false;
  }
  char c;
  string s, s2, s3;
  char s1[256];
  int rn(0), ls1(0), ls2(0), nrun(0), nls(0);
  file.get(c);
  if (c != '{')
    return false;
  while (file >> s and sscanf(s.c_str(), "\"%d\":", &rn) == 1)
  {
    if (_dh_debug)
      PrintInfo(Form("\"%d\": ", rn), true);

    while (file.get(c) and c == ' ')
    {
    };
    if (_dh_debug)
    {
      PrintInfo(Form("%c", c), true);
      assert(c == '[');
    }
    ++nrun;

    bool endrun = false;
    while (!endrun and file >> s >> s2 and (sscanf((s + s2).c_str(), "[%d,%d]%s", &ls1, &ls2, s1) == 3 or (file >> s3 and sscanf((s + s2 + s3).c_str(), "[%d,%d]%s", &ls1, &ls2, s1) == 3)))
    {

      s2 = s1;
      if (s2 == "]")
      {
        file >> s3;
        s2 += s3;
      }

      if (_dh_debug)
        PrintInfo(Form("[%d,%d,'%s']", ls1, ls2, s1), true);

      for (int ls = ls1; ls != ls2 + 1; ++ls)
      {
        _json[rn][ls] = 1;
        ++nls;
      }

      endrun = (s2 == "]," || s2 == "]}");
      if (_dh_debug and !endrun and s2 != ",")
      {
        PrintInfo(string("s1: ") + s2, true);
        assert(s2 == ",");
      }
    } // while ls
    if (_dh_debug)
      PrintInfo("", true);

    if (s2 == "]}")
      continue;
    else if (_dh_debug and s2 != "],")
      PrintInfo(string("s2: ") + s2, true);
    assert(s2 == "],");
  } // while run
  if (s2 != "]}")
  {
    PrintInfo(string("s3: ") + s2, true);
    return false;
  }

  PrintInfo(string("Called LoadJSON() with ") + json + ":", true);
  PrintInfo(Form("Loaded %d good runs and %d good lumi sections", nrun, nls), true);
  return true;
} // LoadJSON

/*
bool DijetHistosFill::LoadJSON()
{
    // Get the JSON files from here:
    // - /eos/user/c/cmsdqm/www/CAF/certification/Collisions22/

    // Golden 1.44/fb
    // string json = "rootfiles/Cert_Collisions2022_355100_356615_Golden.json";
    // Golden JSON, 4.86/fb
    //string json = "rootfiles/Cert_Collisions2022_355100_357550_Golden..json";
    // Golden JSON, 7.67/fb
    //string json = "rootfiles/Cert_Collisions2022_355100_357900_Golden.json";
    // Golden JSON BCDEF, 9.71/fb
    //string json = "rootfiles/Cert_Collisions2022_355100_359812_Golden.json";
    // Golden JSON BCDEF, 14.6/fb
    string json = "rootfiles/Cert_Collisions2022_355100_360491_Golden.json";
    // Golden JSON RunB, 0.0846/fb
    //string json = "rootfiles/Cert_Collisions2022_eraB_355100_355769_Golden.json";
    // Golden JSON RunC, 4.84/fb
    //string json = "rootfiles/Cert_Collisions2022_eraC_355862_357482_Golden.json"
    // Golden JSON RunD, 2.74/fb
    //string json = "rootfiles/Cert_Collisions2022_eraD_357538_357900_Golden.json";
    if (isRun2==1 || isRun2==2)
      json="rootfiles/Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt";
    if (isRun2==3)
      json="rootfiles/Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt";
    if (isRun2==4)
      json="rootfiles/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt";

    if (TString(dataset.c_str()).Contains("2022"))
      json = "rootfiles/Cert_Collisions2022_355100_362760_Golden.json";
    if (TString(dataset.c_str()).Contains("2023"))
      json = "rootfiles/Cert_Collisions2023_366442_370790_Golden.json";

  cout << "Processing LoadJSON() with " + json + " ..." << flush;
  ifstream file(json, ios::in);
  if (!file.is_open()) return false;
  char c;
  string s, s2;
  char s1[256];
  int rn(0), ls1(0), ls2(0), nrun(0), nls(0);
  file.get(c);
  if (c!='{') return false;
  while (file >> s and sscanf(s.c_str(),"\"%d\":",&rn)==1) {
    if (debug) cout << Form("\"%d\": ",rn);

    while (file.get(c) and c==' ') {};
    if (debug) { cout << Form("%c",c) << flush; assert(c=='['); }
    ++nrun;

    bool endrun = false;
    while (!endrun and file >> s >> s2 and sscanf((s+s2).c_str(),"[%d,%d]%s",&ls1,&ls2,s1)==3) {
      if (debug) cout << Form("[%d,%d,%s]",ls1,ls2,s1);

      for (int ls = ls1; ls != ls2+1; ++ls) {
        _json[rn][ls] = 1;
        ++nls;
      }

      s2 = s1;
      endrun = (s2=="]," || s2=="]}");
      if (debug and !endrun and s2!=",") { cout << string("s1: ")+s2 << endl << flush; assert(s2==","); }
    } // while ls
    if (debug) cout << endl;

    if (s2=="]}") continue;
    else if (debug and s2!="],") cout << string("s2: ")+s2 << endl << flush;
    assert(s2=="],");
  } // while run
  //if (s2!="]}") { PrintInfo(string("s3: ")+s2,true); return false; }
  if (s2!="]}") { cout <<  string("s3: ")+s2 << endl; return false; }

  cout << string("\nCalled LoadJSON() with ") + json + ":" << endl;
  cout << Form("Loaded %d good runs and %d good lumi sections",nrun,nls) << endl;
  return true;
} // LoadJSON
*/
/*
bool HistosFill::LoadPUProfiles()
{
  string datafile = ""// jp::pudtpath + (do80 ? "80mb/" : "/") + jp::run + "/pileup_DT.root";
  // string mcfile   = jp::pumcpath + "/pileup_";
  // mcfile += ".root";

  PrintInfo(Form("Processing LoadPUProfiles() using %s and %s ...",datafile.c_str(),mcfile.c_str()),true);

  TDirectory *curdir = gDirectory;
  // Load pile-up files and hists from them
  TFile *f_pudist = new TFile(datafile.c_str(), "READ");
  if (!f_pudist or f_pudist->IsZombie()) { PrintInfo(string("Unable to read PU data file")+datafile,true); return false; }
  // TFile *fpumc = new TFile(mcfile.c_str(),"READ");
  // if (!fpumc or fpumc->IsZombie()) { PrintInfo(string("Unable to read PU MC file")+mcfile,true); return false; }


  // For MC, load the pileup distribution
  _pumc = dynamic_cast<TH1D*>(fpumc->Get("pileupmc")->Clone("pumchelp"));
  if (!_pumc) return false;
  double maxmcpu = _pumc->GetMaximum();
  _pumc->Scale(1.0/maxmcpu);
  int lomclim = _pumc->FindFirstBinAbove(0.01);
  int upmclim = _pumc->FindLastBinAbove (0.01);
  int maxmcbin = _pumc->FindFirstBinAbove(0.999);
  PrintInfo(Form("Maximum bin: %d for MC",maxmcbin),true);
  PrintInfo(Form("Hazardous pu below & above: %f, %f",_pumc->GetBinLowEdge(lomclim),_pumc->GetBinLowEdge(upmclim+1)),true);
  int nbinsmc = _pumc->GetNbinsX();
  int kmc = _pumc->FindBin(33); // Check that pu=33 occurs at the same place as for data


  // For data, load each trigger separately

  for (auto &t : vtrg) {
    TH1D *tmpPU0 = dynamic_cast<TH1D*>(f_pudist->Get(t));
    if (!tmpPU0) {
      PrintInfo(Form("The trigger %s was not found in the DT pileup file!",t),true);
      return false;
    }
    TH1D *tmpPU = dynamic_cast<TH1D*>(tmpPU0->Clone(Form("pu%s",t)));
    int nbinsdt = tmpPU->GetNbinsX();
    int kdt = tmpPU->FindBin(33); // Why bin 33?
    if (kdt!=kmc or nbinsdt!=nbinsmc) {
      PrintInfo("The pileup histogram dt vs mc binning or range do not match (dt left mc right):",true);
      PrintInfo(Form(" Bins: dt:%d mc:%d",nbinsdt,nbinsmc),true);
      PrintInfo(Form(" Pu=33 bin: dt:%d mc:%d",kdt,kmc),true);
      return false;
    }
    double maxdtpu = tmpPU->GetMaximum();
    int lodtlim    = tmpPU->FindFirstBinAbove(maxdtpu/100.0);
    int updtlim    = tmpPU->FindLastBinAbove (maxdtpu/100.0);
    int maxdtbin   = tmpPU->FindFirstBinAbove(0.999*maxdtpu);

    for (int bin = 0; bin < lomclim; ++bin) tmpPU->SetBinContent(bin,0.0); // Set fore-tail to zero
    for (int bin = upmclim+1; bin <= nbinsdt; ++bin) tmpPU->SetBinContent(bin,0.0); // Set aft-tail to zero

    PrintInfo(Form("Maximum bin: %d for DT trg %s",maxdtbin,t),true);
    PrintInfo(Form("Hazardous pu below & above: %f, %f",tmpPU->GetBinLowEdge(lodtlim),tmpPU->GetBinLowEdge(updtlim+1)),true);
    tmpPU->Divide(_pumc);
    // if (do80) _pudist80[t] = tmpPU;
    _pudist[t] = tmpPU;
  }
  PrintInfo("Finished processing pileup histos!",true);

  curdir->cd();
  return true;
} // LoadPUProfiles
*/
