import ROOT
import json
import time

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

def convert_to_ranges(lumi_blocks):
    """Convert a set of luminosity blocks to a list of ranges."""
    sorted_lumis = sorted(lumi_blocks)
    ranges = []
    start = sorted_lumis[0]
    end = sorted_lumis[0]
    
    for lumi in sorted_lumis[1:]:
        if lumi == end + 1:
            end = lumi
        else:
            ranges.append([start, end])
            start = lumi
            end = lumi
    
    ranges.append([start, end])
    return ranges

def write_to_json(data, output_file):
    """Writes data to a JSON file."""
    with open(output_file, 'w') as f:
        json.dump(data, f, indent=4, sort_keys=True)

def main(file_list_path, output_json_path):
    """Main function to read, extract, and write data."""
    file_list = read_file_list(file_list_path)
    combined_data = {}
    
    total_files = len(file_list)
    print(f"Total files to process: {total_files}")

    start_time = time.time()
    
    for i, root_file in enumerate(file_list, 1):
        print(f"Processing file {i}/{total_files}: {root_file}")
        run_lumi_blocks = extract_run_lumi_blocks(root_file)
        for run, lumis in run_lumi_blocks.items():
            if run not in combined_data:
                combined_data[run] = set()
            combined_data[run].update(lumis)

    # Convert sets to lists of ranges for JSON serialization
    combined_data_ranges = {run: convert_to_ranges(lumis) for run, lumis in combined_data.items()}
    
    write_to_json(combined_data_ranges, output_json_path)

    end_time = time.time()
    elapsed_time = end_time - start_time
    print(f"Processing completed in {elapsed_time:.2f} seconds.")
    
if __name__ == "__main__":
    main('part2.txt', 'part2.json')

