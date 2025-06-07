# This script generates a C++ header file containing the network topology
# The result is single graph that can be then used with PREDEFINED_TOPOLOGY analysis
# (the one thata uses SocialNetwork.h)

# Path to the file with the network topology
input_file = 'Resources/Example_topologies/S1Anonymized_neighbors.csv'
# Path to the output file
output_file = 'AutomaticallyGeneratedNetworkTopology.h'

network_topology = []

with open(input_file, 'r') as infile:
	for row in infile:
		if len(row) <= 1:
			break
		temporary_array = []
		for value in row[:-1].split(','):
			temporary_array.append(int(value) - 1)	# we change the agents IDs to start from 0
		network_topology.append(temporary_array)

max_number_of_neighbors = max([len(list) for list in network_topology])

with open(output_file, 'w') as outfile:
	outfile.write('#pragma once\n\n')
	outfile.write('#include <array>\n\n')
	outfile.write('namespace NetworkTopology {\n\n')
	outfile.write(f'constexpr std::size_t NETWORK_SIZE = {len(network_topology)};\t// the IDs of the agents are from 0 to (NETWORK_SIZE - 1)\n\n')
	outfile.write(f'constexpr std::size_t MAX_NEIGHBORS = {max_number_of_neighbors};\n\n')
	outfile.write('// create an array of numbers of neighbors for each agent\n')
	outfile.write('constexpr std::array <std::size_t, NETWORK_SIZE> NumberOfNeighbors = {')
	for i, list in enumerate(network_topology):
		outfile.write(f'{len(list)}{"," if i < (len(network_topology) - 1) else ""}')
	outfile.write('};\n\n')
	outfile.write('// create an array of lists of neighbors for each agent\n')
	outfile.write('constexpr std::array <std::array <std::size_t, MAX_NEIGHBORS>, NETWORK_SIZE> Network = \n')
	outfile.write('\t{\n')
	for i, list in enumerate(network_topology):
		outfile.write('\t\tstd::array <std::size_t, MAX_NEIGHBORS> {')
		for j, value in enumerate(list):
			outfile.write(f'{value}{"," if j < (max_number_of_neighbors - 1) else ""}')
		for j in range(max_number_of_neighbors - len(list)):
			outfile.write(f'{0}{"," if (j + len(list)) < (max_number_of_neighbors - 1) else ""}')
		outfile.write('}')
		outfile.write(f'{"," if i != len(network_topology) - 1 else ""}\n')

	outfile.write('\t};\n')
	outfile.write('}\n')
