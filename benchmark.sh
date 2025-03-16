#!/bin/bash
#SBATCH --partition=Centaurus
#SBATCH --job-name=graph-crawler-test
#SBATCH --output=test_graph.txt
#SBATCH --time=01:00:00 
#SBATCH --mem=32G

# Output file where results will be saved
output_file="test_graph.txt"

# Clear the output file before appending new results
> "$output_file"

# Array of test cases with starting nodes and depths
test_cases=(
  "Tom Hanks 2"
  "Leonardo DiCaprio 3"
  "Brad Pitt 1"
)

# Loop through each test case and run the graph-crawler program
for test_case in "${test_cases[@]}"; do
  # Extract node and depth correctly
  node=$(echo "$test_case" | awk '{$NF=""; print $0}' | sed 's/[[:space:]]*$//')  # Get all but last word
  depth=$(echo "$test_case" | awk '{print $NF}')  # Get last word as depth

  # Run the graph-crawler with the node and depth as arguments and redirect output to the file
  echo "Running BFS with node: $node and depth: $depth" >> "$output_file"
  ./graph-crawler "$node" "$depth" >> "$output_file"
  echo "--------------------------------" >> "$output_file"
done
