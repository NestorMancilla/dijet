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
    '2024_skim', '2024I_Skim', '2024H_Skim',
    '2024F_ECAL_CC_1', '2024F_ECAL_CC_2', '2024F_ECAL_CC_3', '2024F_ECAL_CC_4',
    '2024F_ECAL2_1', '2024F_ECAL2_2', '2024F_ECAL2_3', '2024F_ECAL2_4',
    'Winter24MG_1', 'Winter24MG_2', 'Winter24MG_3', 'Winter24MG_4', 'Winter24MG_5',
    'Summer24MG_1', 'Summer24MG_2', 'Summer24MG_3', 'Summer24MG_4', 'Summer24MG_5', 'Summer24MG_oneHT', 'Summer24MC_FlatJMEN',
    'Winter25MC_Flat2022', 'Summer24MC_NoPU', 'Winter25MC_NoPU', 'Summer24MC_NoPU_JME', "Winter25MC_JME",
    'Summer24MC_SingleNeutrino',
    'Winter24MGV14_1', 'Winter24MGV14_2', 'Winter24MGV14_3', 'Winter24MGV14_4', 'Winter24MGV14_5', 'Winter24MGV14_OneHTFile',
    'QCDFlatECAL_1Sig', 'QCDFlatECAL_2Sig', 'QCDFlatECAL_3Sig', 'QCDFlatECAL_4Sig', 'QCDFlatECAL_Baseline', 'QCDFlatECAL_Zero',
    '2025B', '2025B_ZB', '2025C', '2025C_ZB',
    'Winter25MG_1', 'Winter25MG_2', 'Winter25MG_3', 'Winter25MG_4', 'Winter25MG_5'
]

# Run 3 is all samples with year 2023 and 2022 from the full IOV_list
run3_IOV_list = [x for x in IOV_list if '2023' in x or '2022' in x or 'Summer22' in x]
run3_DT = [x for x in IOV_list if '2023' in x or '2022' in x]
run3_MC = [x for x in IOV_list if 'Summer22' in x]
summer23_MC = [x for x in IOV_list if 'Summer23' in x]

version = 'v134_v2'
#version = 'v120'

IOV_input = []

parser = argparse.ArgumentParser(description='Run all IOVs')

# The user can pass the IOV list, version, max number of files as an argument
parser.add_argument('--IOV_list', nargs='+', default=IOV_input)
parser.add_argument('--version', default=version)
parser.add_argument('--max_files', default=9999)
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

# Check that the version directory exists, if not create it
if not os.path.exists('rootfiles/'+version):
    os.makedirs('rootfiles/'+version)

if not os.path.exists('logs/'+version):
    os.makedirs('logs/'+version)

for iov in IOV_input:
    print(f"Process DijetHistosFill.C+g for IOV {iov}")
    # os.system(f"ls -ltrh rootfiles/jmenano_mc_out_{iov}_{version}.root")
    # os.system(f"ls -ltrh rootfiles/jmenano_data_out_{iov}_{version}.root")
    # os.system(f"ls -ltrh logs/log_{iov}_{version}.txt")
    #os.system(f"nohup root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files})' > logs/{version}/log_{iov}_{version}.txt &")
    os.system(f"nohup time root -l -b -q 'make/mk_DijetHistosFill.C(\"{iov}\",\"{version}\",{max_files})' > logs/{version}/log_{iov}_{version}.txt &")
    print(f" => Follow logging with 'tail -f logs/{version}/log_{iov}_{version}.txt'")

#    os.system("fs flush")
#    wait()
#    time.sleep(sleep_time)
