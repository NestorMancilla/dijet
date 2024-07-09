import ROOT
import json

def read_file_list(file_path):
    """Reads a list of file paths from a text file."""
    with open(file_path, 'r') as f:
        return [line.strip() for line in f.readlines()]

def extract_run_lumi_blocks(root_file_path):
    """Extracts run numbers and luminosity blocks from a ROOT file."""
    run_lumi_blocks = {}
    file = ROOT.TFile.Open(root_file_path)
    
    if not file or file.IsZombie():
        print(f"Error opening file {root_file_path}")
        return run_lumi_blocks
    
    tree = file.Get("Events")
    if not tree:
        print(f"No 'Events' tree in {root_file_path}")
        return run_lumi_blocks

    for event in tree:
        run = event.run
        lumi = event.luminosityBlock
        if run not in run_lumi_blocks:
            run_lumi_blocks[run] = set()
        run_lumi_blocks[run].add(lumi)
    
    file.Close()
    return run_lumi_blocks

def write_to_json(data, output_file):
    """Writes data to a JSON file."""
    with open(output_file, 'w') as f:
        json.dump(data, f, indent=4, sort_keys=True)

def main(file_list_path, output_json_path):
    """Main function to read, extract, and write data."""
    file_list = read_file_list(file_list_path)
    combined_data = {}
    
    for root_file in file_list:
        run_lumi_blocks = extract_run_lumi_blocks(root_file)
        for run, lumis in run_lumi_blocks.items():
            if run not in combined_data:
                combined_data[run] = set()
            combined_data[run].update(lumis)
    
    # Convert sets to lists for JSON serialization
    for run in combined_data:
        combined_data[run] = list(combined_data[run])
    
    write_to_json(combined_data, output_json_path)
    
if __name__ == "__main__":
    main('dataFiles_2024CS_XS_v2.txt', 'output.json')

