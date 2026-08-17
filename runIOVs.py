#! /usr/bin/python
import os
import argparse

max_files = 9999

IOV_list= ['UL2016BCD','UL2016EF','UL2016GH',
   'UL2017B','UL2017C','UL2017D','UL2017E','UL2017F',
    'UL2018A','UL2018B','UL2018C', 'UL2018D',
    'UL2018D1','UL2018D2',
    'UL2016APVMG','UL2016MG','UL2017MG', 'UL2018MC',
    'UL2016BCD_ZB','UL2016EF_ZB','UL2016GH_ZB',
    'UL2017B_ZB','UL2017C_ZB','UL2017D_ZB','UL2017E_ZB','UL2017F_ZB',
    'UL2018A_ZB','UL2018B_ZB','UL2018C_ZB', 'UL2018D_ZB',
    '2022C','2022D','2022E', '2022F', '2022G', # '2022F1','2022F2',
    '2022C_ZB','2022D_ZB','2022E_ZB','2022F_ZB','2022G_ZB',
    '2022D_prompt', '2022D_ZB_prompt', '2022C_prompt', '2022C_ZB_prompt',
    '2022C_nib1', '2022D_nib1', '2022E_nib1', '2022F_nib1', '2022G_nib1',
    '2022C_nib1_ZB', '2022D_nib1_ZB', '2022E_nib1_ZB', '2022F_nib1_ZB', '2022G_nib1_ZB',
    '2023Cv4', '2023D', '2023BCv123', '2023Cv123', '2023Cv123_ZB',
    '2023BCv123_ZB','2023Cv4_ZB','2023D_ZB',
    '2023Bv1_nib1', '2023Cv1_nib1', '2023Cv2_nib1', '2023Cv3_nib1', '2023Cv4_nib1', '2023Cv4_nib2', '2023Dv1_nib1', '2023Dv2_nib1',
    '2023Bv1_nib1_ZB', '2023Cv1_nib1_ZB', '2023Cv2_nib1_ZB', '2023Cv3_nib1_ZB', '2023Cv4_nib1_ZB', '2023Cv4_nib2_ZB', '2023Dv1_nib1_ZB', '2023Dv2_nib1_ZB',
    '2023Cv123_prompt', '2023Cv123_ZB_prompt', '2023Cv4_prompt', '2023Cv4_ZB_prompt', '2023D_prompt', '2023D_ZB_prompt',
    'UL2018MG_1', 'UL2018MG_2', 'UL2018MG_3', 'UL2018MG_4', 'UL2018MG_5',
    'Summer22MC_Flat2018',
    'Summer22MG1','Summer22MG2',
    'Summer22MG_1', 'Summer22MG_2', 'Summer22MG_3', 'Summer22MG_4', 'Summer22MG_5',
    'Summer22EEMG1','Summer22EEMG2','Summer22EEMG3','Summer22EEMG4',
    'Summer22EEMG_1', 'Summer22EEMG_2', 'Summer22EEMG_3', 'Summer22EEMG_4', 'Summer22EEMG_5',
    'Summer22EEMG_full', 'Summer22MG_full',
    'Summer23MG_1', 'Summer23MG_2', 'Summer23MG_3', 'Summer23MG_4', 'Summer23MG',
    'Summer23MGBPix_1', 'Summer23MGBPix_2', 'Summer23MGBPix_3', 'Summer23MGBPix_4', 'Summer23MGBPix',
    'Summer23MG_test', 'Summer23MGBPix_test',
    'Summer23MG_new_1', 'Summer23MG_new_2', 'Summer23MG_new_3', 'Summer23MG_new_4',
    'TestSummer23MGBPix', 'Summer23MCFlat', 'Summer23MCBPixFlat',
    'Summer23MCBPix_Cas', 'Summer23MC_Cas',
    'Winter24MCFlat', 'Winter24MCFlat_Sv10', 'Winter24MCFlat_Sv9',
    '2024B', '2024B_JME', '2024B_ZB', '2024C', '2024C_JME', '2024C_ZB', '2024D', '2024D_ZB', '2024Ev1', '2024Ev1_ZB', '2024Ev2', '2024Ev2_ZB',
    '2024F', '2024F_ZB', '2024G', '2024G_ZB',
    '2024F_1', '2024F_2', '2024F_3', '2024F_4',
    '2024B_nib1', '2024C_nib1', '2024D_nib1', '2024Ev1_nib1', '2024Ev2_nib1', '2024H_nib1', '2024Iv1_nib1', '2024Iv2_nib1',
    '2024F_nib1', '2024F_nib2', '2024F_nib3', '2024G_nib1', '2024G_nib2',
    '2024B_nib1_ZB', '2024C_nib1_ZB', '2024D_nib1_ZB', '2024Ev1_nib1_ZB', '2024Ev2_nib1_ZB', '2024H_nib1_ZB', '2024Iv1_nib1_ZB', '2024Iv2_nib1_ZB',
    '2024F_nib1_ZB', '2024F_nib2_ZB', '2024F_nib3_ZB', '2024G_nib1_ZB', '2024G_nib2_ZB',
    '2024F_nib1_ZeroBias', '2024F_nib2_ZeroBias', '2024F_nib3_ZeroBias',
    '2024G_1', '2024G_2', '2024G_3', '2024G_4', '2024G_5', '2024H_NT',
    '2024H', '2024H_ZB', '2024Iv1', '2024Iv1_ZB', '2024Iv2', '2024Iv2_ZB',
    '2024BR', '2024CR', '2024Crs', '2024CS', '2024CT', '2024F_TeVJet', '2024F_JetHT',
    '2024C_Rp', '2024C_Rp_ZB', '2024D_Rp', '2024D_Rp_ZB', '2024E_Rp', '2024E_Rp_ZB',
    '2024I_ZB_HCPF1x', '2024I_ZB_HCPF2x', '2024I_ZB_HCPF3x', '2024I_ZB_HCPF4x', '2024I_ZB_HCPF5x', '2024I_ZB_HCPFSpecial', '2024I_ZB_Special',
    '2024I_ZB',
    '2024_skim',
    '2024C_Rp_Skim_0', '2024C_Rp_Skim_1', '2024C_Rp_Skim_ZB',
    '2024D_Rp_Skim_0', '2024D_Rp_Skim_1', '2024D_Rp_Skim_ZB',
    '2024E_Rp_Skim_0', '2024E_Rp_Skim_1', '2024E_Rp_Skim_ZB',
    '2024H_Skim_0', '2024H_Skim_1', '2024H_Skim_ZB',
    '2024I_Skim_0', '2024I_Skim_1', '2024I_Skim_ZB',
    '2024F_nib1_Skim_0', '2024F_nib1_Skim_1', '2024F_nib1_Skim_ZB',
    '2024F_nib2_Skim_0', '2024F_nib2_Skim_1', '2024F_nib2_Skim_ZB',
    '2024F_nib3_Skim_0', '2024F_nib3_Skim_1', '2024F_nib3_Skim_ZB',
    '2024G_nib1_Skim_0', '2024G_nib1_Skim_1', '2024G_nib1_Skim_ZB',
    '2024G_nib2_Skim_0', '2024G_nib2_Skim_1', '2024G_nib2_Skim_ZB',
    '2024C_Rp_JME_Skim_0', '2024C_Rp_JME_Skim_1', '2024C_Rp_JME_Skim_ZB',
    '2024D_Rp_JME_Skim_0', '2024D_Rp_JME_Skim_1', '2024D_Rp_JME_Skim_ZB',
    '2024E_Rp_JME_Skim_0', '2024E_Rp_JME_Skim_1', '2024E_Rp_JME_Skim_ZB',
    '2024F_nib1_JME_Skim_0', '2024F_nib1_JME_Skim_1', '2024F_nib1_JME_Skim_ZB',
    '2024F_nib2_JME_Skim_0', '2024F_nib2_JME_Skim_1', '2024F_nib2_JME_Skim_ZB',
    '2024F_nib3_JME_Skim_0', '2024F_nib3_JME_Skim_1', '2024F_nib3_JME_Skim_ZB',
    '2024G_nib1_JME_Skim_0', '2024G_nib1_JME_Skim_1', '2024G_nib1_JME_Skim_ZB',
    '2024G_nib2_JME_Skim_0', '2024G_nib2_JME_Skim_1', '2024G_nib2_JME_Skim_ZB',
    '2024H_JME_Skim_0', '2024H_JME_Skim_1', '2024H_JME_Skim_ZB',
    '2024I_JME_Skim_0', '2024I_JME_Skim_1', '2024I_JME_Skim_ZB',
    '2024F_ECAL_CC_1', '2024F_ECAL_CC_2', '2024F_ECAL_CC_3', '2024F_ECAL_CC_4',
    '2024F_ECAL2_1', '2024F_ECAL2_2', '2024F_ECAL2_3', '2024F_ECAL2_4',
    'Winter24MG_1', 'Winter24MG_2', 'Winter24MG_3', 'Winter24MG_4', 'Winter24MG_5',
    'Summer24MG_1', 'Summer24MG_2', 'Summer24MG_3', 'Summer24MG_4', 'Summer24MG_5', 'Summer24MG_oneHT', 'Summer24MC_FlatJMEN',
    'Summer24MG_6', 'Summer24MG_7', 'Summer24MG_8', 'Summer24MG_9', 'Summer24MG_10', 'Summer24MC_Flat', 'Summer24MG_11',
    'Winter25MC_Flat2022', 'Summer24MC_NoPU', 'Winter25MC_NoPU', 'Summer24MC_NoPU_JME', "Winter25MC_JME",
    'Winter24MGV14_1', 'Winter24MGV14_2', 'Winter24MGV14_3', 'Winter24MGV14_4', 'Winter24MGV14_5', 'Winter24MGV14_OneHTFile',
    'QCDFlatECAL_1Sig', 'QCDFlatECAL_2Sig', 'QCDFlatECAL_3Sig', 'QCDFlatECAL_4Sig', 'QCDFlatECAL_Baseline', 'QCDFlatECAL_Zero',
    '2025B', '2025B_ZB', '2025Cv1', '2025Cv1_ZB', '2025Cv2', '2025Cv2_ZB', '2025D', '2025D_ZB', '2025E', '2025E_ZB', '2025F', '2025F_ZB',
    '2025D_1', '2025D_1_ZB', '2025D_2', '2025D_2_ZB', '2025E_1', '2025E_2', '2025Fv1_1', '2025Fv1_2', '2025Fv1_ZB', '2025Fv2_1', '2025Fv2_2', '2025Fv2_ZB', '2025Fv1_11', '2025Fv1_12', '2025Fv1_13', '2025Fv1_21', '2025Fv1_22', '2025Fv1_23',
    '2025G_11', '2025G_12', '2025G_21', '2025G_22', '2025G_ZB', '2025D_11', '2025D_12', '2025D_21', '2025D_22',
    '2025G', '2025G_ZB',
    '2025C_Trk', '2025C_Trk_ZB',
    'Winter25MG_1', 'Winter25MG_2', 'Winter25MG_3', 'Winter25MG_4', 'Winter25MG_5', 'Summer24MC_Flat2022',
    'Winter25MC_Flat22', 'Summer24MC_Flat22_NoDeepCore', 'Summer24MC_Flat22_Base', 'Summer24MC_Flat22_NoPU',
    'Summer24MC_SingleNeutrino', 'Summer24MC_SingleNeutrino_FlatPU', 'Summer24MC_SingleNeutrino_NoPU',
    'Winter25MC_Flat_EEZS9p5',
    'Winter26MC_Flat22',
    'Summer24MC_Flat22_Herwig',
    'Winter25MG_v15_1', 'Winter25MG_v15_2', 'Winter25MG_v15_3', 'Winter25MG_v15_4', 'Winter25MG_v15_5',
    'Winter25MG_v15_6', 'Winter25MG_v15_7', 'Winter25MG_v15_8', 'Winter25MG_v15_9', 'Winter25MG_v15_10',
    'Winter25MG_v15_11',
    'Summer24MC_Flat22_JME', 'Summer24MG_JME_1', 'Summer24MG_JME_2', 'Summer24MG_JME_3', 'Summer24MG_JME_4',
    'Summer22MC_Flat22_JME', 'Summer23MC_Flat22_JME',
    'Summer22MC_base_1M', 'Summer22MC_hadCalibOff_1M',
    'Summer24MG_JME_5', 'Summer24MG_JME_6', 'Summer24MG_JME_7', 'Summer24MG_JME_8', 'Summer24MG_JME_9',
    'Summer24MG_JME_10', 'Summer24MG_JME_11',
    'Summer24MG_2025_1', 'Summer24MG_2025_2', 'Summer24MG_2025_3', 'Summer24MG_2025_4',
    'Summer24MG_2025_5', 'Summer24MG_2025_6', 'Summer24MG_2025_7', 'Summer24MG_2025_8',
    'Summer24MG_2025_9', 'Summer24MG_2025_10', 'Summer24MG_2025_11',
    'Summer24MG_2026_1', 'Summer24MG_2026_2', 'Summer24MG_2026_3', 'Summer24MG_2026_4', 
    'Summer24MG_2026_5', 'Summer24MG_2026_6', 'Summer24MG_2026_7', 'Summer24MG_2026_8',
    'Summer24MG_2026_9', 'Summer24MG_2026_10', 'Summer24MG_2026_11',
    'Winter26MG_1', 'Winter26MG_2', 'Winter26MG_3', 'Winter26MG_4', 'Winter26MG_5', 'Winter26MG_6',
    'Winter26MG_7', 'Winter26MG_8', 'Winter26MG_9', 'Winter26MG_10', 'Winter26MG_11',
    'Summer20UL16APVMG_1', 'Summer20UL16APVMG_2', 'Summer20UL16APVMG_3', 'Summer20UL16APVMG_4', 'Summer20UL16APVMG_5',
    'Summer20UL16APVMG_6', 'Summer20UL16APVMG_7', 'Summer20UL16APVMG_8', 'Summer20UL16APVMG_9',
    'Summer20UL16MG_1', 'Summer20UL16MG_2', 'Summer20UL16MG_3', 'Summer20UL16MG_4', 'Summer20UL16MG_5',
    'Summer20UL16MG_6', 'Summer20UL16MG_7', 'Summer20UL16MG_8', 'Summer20UL16MG_9',
    'Summer20UL17MG_1', 'Summer20UL17MG_2', 'Summer20UL17MG_3', 'Summer20UL17MG_4', 'Summer20UL17MG_5',
    'Summer20UL17MG_6', 'Summer20UL17MG_7', 'Summer20UL17MG_8', 'Summer20UL17MG_9',
    'Summer20UL18MG_1', 'Summer20UL18MG_2', 'Summer20UL18MG_3', 'Summer20UL18MG_4', 'Summer20UL18MG_5',
    'Summer20UL18MG_6', 'Summer20UL18MG_7', 'Summer20UL18MG_8', 'Summer20UL18MG_9',
    '2025Fv1_Skim_11', '2025Fv1_Skim_12', '2025Fv1_Skim_13', '2025Fv1_Skim_21', '2025Fv1_Skim_22', '2025Fv1_Skim_23',
    '2025Fv2_Skim_1', '2025Fv2_Skim_2',
    '2025Fv1', '2025Fv1_ZB', '2025Fv2', '2025Fv2_ZB'
    '2025Cv1_Skim_1', '2025Cv1_Skim_2', '2025Cv1_Skim_ZB',
    '2025Cv1_JME_0', '2025Cv1_JME_1', '2025Cv2_JME_0', '2025Cv2_JME_1', '2025D_JME_0', '2025D_JME_1',
    '2025E_JME_0', '2025E_JME_1', '2025Fv1_JME_0', '2025Fv1_JME_1', '2025Fv2_JME_0', '2025Fv2_JME_1',
    '2025G_JME_0', '2025G_JME_1',
    '2025Cv1_JME_ZB', '2025Cv2_JME_ZB', '2025D_JME_ZB', '2025E_JME_ZB', '2025Fv1_JME_ZB', '2025Fv2_JME_ZB', '2025G_JME_ZB',
    '2026A_1', '2026A_0', '2026A_ZB', '2026Bnib1_0', '2026Bnib1_1', '2026Bnib2_0', '2026Bnib2_1', '2026Bnib1_ZB', '2026Bnib2_ZB',
    'Summer24MC_withNP', 'Summer24MC_NPoff',
    'Summer24MC_Flat22_NoPFH', 'Summer24MC_Flat22_NoPFH_v2',
    '2026B_0', '2026B_1', '2026B_ZB',
    '2026Bnib2_ZB_16', '2026Bnib2_ZB_15', '2026Bnib2_ZB_14', '2026Bnib2_ZB_13', '2026Bnib2_ZB_12',
    '2026Bnib2_ZB_11',
    '2026Bnib2_03', '2026Bnib2_02', '2026Bnib2_01',
    '2026Bnib2_11', '2026Bnib2_12', '2026Bnib2_13', '2026Bnib2_14',
    '2026Bnib2_121', '2026Bnib2_122',
    '2026B_ZB_part_05', '2026B_ZB_part_04', '2026B_ZB_part_03', '2026B_ZB_part_02',
    '2026B_ZB_part_01', '2026B_ZB_part_00',
    '2026B_0_part_05', '2026B_0_part_04', '2026B_0_part_03', '2026B_0_part_02',
    '2026B_0_part_01', '2026B_0_part_00',
    '2026B_1_part_05', '2026B_1_part_04', '2026B_1_part_03', '2026B_1_part_02',
    '2026B_1_part_01', '2026B_1_part_00', 
    '2026C_0', '2026C_1', '2026C_2', '2026C_3', '2026C_4', '2026C_5', '2026C_ZB',
    '2026C_01', '2026C_11', '2026C_21', '2026C_31', '2026C_41', '2026C_51', '2026C_ZB1',
    '2026D_0', '2026D_1', '2026D_ZB',
    '2026B_JME_0', '2026B_JME_1', '2026B_JME_ZB', 
    '2026C_JME_0', '2026C_JME_1', '2026C_JME_2', '2026C_JME_3', '2026C_JME_4', '2026C_JME_5', '2026C_JME_ZB',
    '2026D_JME_0', '2026D_JME_1', '2026D_JME_ZB',
    '2016B_HIPM', '2016B_HIPM_ZB', '2016C_HIPM', '2016C_HIPM_ZB', '2016D_HIPM', '2016D_HIPM_ZB',
    '2016E_HIPM', '2016E_HIPM_ZB', '2016F_HIPM', '2016F_HIPM_ZB',
    '2016F', '2016F_ZB', '2016G', '2016G_ZB', '2016H', '2016H_ZB',
    '2017B', '2017B_ZB', '2017C', '2017C_ZB', '2017D', '2017D_ZB', '2017E', '2017E_ZB', '2017F', '2017F_ZB',
    '2018A', '2018A_ZB', '2018B', '2018B_ZB', '2018C', '2018C_ZB', '2018D', '2018D_ZB'
]

# Run 3 is all samples with year 2023 and 2022 from the full IOV_list
run3_IOV_list = [x for x in IOV_list if '2023' in x or '2022' in x or 'Summer22' in x]
run3_DT = [x for x in IOV_list if '2023' in x or '2022' in x]
run3_MC = [x for x in IOV_list if 'Summer22' in x]
summer23_MC = [x for x in IOV_list if 'Summer23' in x]

version = 'v172'
#version = 'v120'

IOV_input = []

parser = argparse.ArgumentParser(description='Run all IOVs')

# The user can pass the IOV list, version, max number of files as an argument
parser.add_argument('--IOV_list', nargs='+', default=IOV_input, help='IOV to process i.e 2024C')
parser.add_argument('--version', default=version, help='Version of the process i.e. v150')
parser.add_argument('--max_files', default=9999, help='Max number of root files per IOV list')
parser.add_argument('--chunk', type=int, default=-1, help='Chunk number for splitting jobs')
args = parser.parse_args()

if args.IOV_list:
    if 'all' in args.IOV_list:
        IOV_input = IOV_list
    elif 'run3' in args.IOV_list:
        IOV_input = run3_IOV_list
    elif 'run3DT' in args.IOV_list:
        IOV_input = run3_DT
    elif 'run3MC' in args.IOV_list:
        IOV_input = run3_MC
    elif 'summer23MC' in args.IOV_list:
        IOV_input = summer23_MC
    elif 'test' in args.IOV_list:
        IOV_input = run3_IOV_list[1:5]
        max_files = 4
        version = version+'_test'
    else:
        # Check that all IOVs passed are in the list
        for iov in args.IOV_list:
            if iov not in IOV_list:
                print('IOV '+iov+' not in list of IOVs')
                exit()
            else:
                IOV_input.append(iov)
else:
    print('No IOV list passed')
    exit()
    
if (args.version) and ('test' not in args.IOV_list):
    version = args.version

if args.max_files and ('test' not in args.IOV_list):
    max_files = args.max_files
    
print('IOVs to run: ', IOV_input)

EOS_BASE = "/eos/user/n/nmancill/Helsinki/dijet/Rootfiles_afs" # <-- Update if you're not Nestor :D

eos_version_dir = f"{EOS_BASE}/{version}"

# Ensure the EOS directory exists
if not os.path.exists(eos_version_dir):
    os.makedirs(eos_version_dir)

# Keep log files locally in AFS
if not os.path.exists('logs/'+version):
    os.makedirs('logs/'+version)

# Pass the EOS path and chunk number to C++ via environment variables!
os.environ["DIJET_OUTDIR"] = eos_version_dir
os.environ["DIJET_CHUNK"] = str(args.chunk)

for iov in IOV_input:
    print(f"Process DijetHistosFill.C+g for IOV {iov} (Chunk {args.chunk}) -> Saving to EOS")
    
    os.system(f"root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files},{args.chunk})'")


    #os.system(f"nohup root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files})' > logs/{version}/log_{iov}_{version}.txt &")
    #print(f" => Follow logging with 'tail -f logs/{version}/log_{iov}_{version}.txt'")
    #os.system(f"root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files})' > /afs/cern.ch/user/n/nmancill/Helsinki/dijet/logs/{version}/log_{iov}_{version}.txt")
    #os.system(f"root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files})'")
    #print(f" => Follow logging with 'tail -f logs/{version}/log_{iov}_{version}.txt'")
    #os.system(f"nohup time root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files})' > logs/{version}/log_{iov}_{version}.txt &")
    #print(f" => Follow logging with 'tail -f logs/{version}/log_{iov}_{version}.txt'")
#    os.system("fs flush")
#    wait()
#    time.sleep(sleep_time)
