import csv
from collections import defaultdict

# List of triggers to consider
triggers_to_consider = [
    "HLT_PFJet40",
    "HLT_PFJet60",
    "HLT_PFJet80",
    "HLT_PFJet140",
    "HLT_PFJet200",
    "HLT_PFJet260",
    "HLT_PFJet320",
    "HLT_PFJet400",
    "HLT_PFJet450",
    "HLT_PFJet500"
]

luminosity_sum = defaultdict(float)

with open('/media/storage/nestorma/dijet/textFiles/Prompt24/LumiperTrg/lumi_HLT_Golden_2024I_PFJet.csv', 'r') as file:
    reader = csv.reader(file, delimiter=',')
    
    for _ in range(3):
        next(reader)
    
    for row in reader:
        if len(row) < 6 or not row[5].replace('.', '', 1).isdigit():
            continue  # Skip rows that do not have enough columns or where the recorded luminosity is not a valid float
        
        hltpath = row[3]
        recorded_luminosity = float(row[5])
        
        # Check if the trigger is in the list of triggers to consider
        for trigger in triggers_to_consider:
            if hltpath.startswith(trigger + '_'):
                luminosity_sum[trigger] += recorded_luminosity
                break

with open('/media/storage/nestorma/dijet/src/luminosity_2024I_sum.txt', 'w') as f:
    for trigger in triggers_to_consider:
        total_luminosity = luminosity_sum[trigger]
        f.write(f"{trigger}: {total_luminosity}\n")


