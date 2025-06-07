# This script generates a C++ header file containing predefined network topologies.
# It reads network topology data from CSV files and formats it into a C++ header file.
# The result graph is Watts-Strogatz small-world network. It can used with THEORETICAL_GRAPH anaylsis
# (the one that uses TheoreticalGraph.h)

output_file = 'AutomaticallyGeneratedNetworkTopologies.h'

N = 1000
k = 8
beta = "0.05" # user may use different decimal precision, therefore we use string
number_of_topologies_to_simulate = 10

def get_input_file(index: int) -> str:
	return f'Resources/Example_topologies/WS_N_{N}_beta_{beta}_k_{k}_graph_{index + 1}.csv'

def get_as_array(list_of_elements, size = None) -> str:
	number_of_elements = len(list_of_elements) if size is None else size
	temporary_text = '{'
	for idx in range(number_of_elements):
		temporary_text += f'{list_of_elements[idx] if idx < len(list_of_elements) else 0}'
		if idx != number_of_elements - 1:
			temporary_text += ', '
	temporary_text += '}'
	return temporary_text

network_topologies = []

for i in range(number_of_topologies_to_simulate):
	network_topologies.append([])
	with open(get_input_file(i), 'r') as infile:
		for row in infile:
			if len(row) <= 1:
				break
			temporary_array = []
			for value in row[:-1].split(','):
				temporary_array.append(int(value) - 1)	# we change the agents IDs to start from 0
			network_topologies[i].append(temporary_array)

max_number_of_neighbors = max([max([len(list) for list in topology]) for topology in network_topologies])

with open(output_file, 'w') as outfile:
	outfile.write('#pragma once\n\n')
	outfile.write('#include <array>\n\n')
	outfile.write('namespace NetworkTopology {\n\n')
	outfile.write(f'constexpr std::size_t NETWORK_SIZE = {N};\t// the IDs of the agents are from 0 to (NETWORK_SIZE - 1)\n\n')
	outfile.write(f'constexpr std::size_t NUMBER_OF_TOPOLOGIES_TO_SIMULATE = {number_of_topologies_to_simulate};\n\n')
	outfile.write(f'constexpr std::size_t MAX_NEIGHBORS = {max_number_of_neighbors};\n\n')
	outfile.write('// create an array of numbers of neighbors for each agent\n')
	outfile.write('constexpr std::array <std::array <std::size_t, NETWORK_SIZE>, NUMBER_OF_TOPOLOGIES_TO_SIMULATE> NumbersOfNeighbors = {\n\t')
	for idx, topology in enumerate(network_topologies):
		outfile.write(f'std::array <std::size_t, NETWORK_SIZE> {get_as_array([len(neighbors) for neighbors in topology])}')
		if idx != number_of_topologies_to_simulate - 1:
			outfile.write(',\n\t')
	outfile.write('\n};\n\n')
	outfile.write('// create an array of lists of neighbors for each agent\n')
	outfile.write('constexpr std::array <std::array <std::array <std::size_t, MAX_NEIGHBORS>, NETWORK_SIZE>, NUMBER_OF_TOPOLOGIES_TO_SIMULATE> Networks = {\n\t')
	for idx, topology in enumerate(network_topologies):
		outfile.write('std::array <std::array <std::size_t, MAX_NEIGHBORS>, NETWORK_SIZE> {\n\t\t')
		for idy, list_of_agents in enumerate(topology):
			outfile.write(f'std::array <std::size_t, MAX_NEIGHBORS> {get_as_array(list_of_agents, max_number_of_neighbors)}')
			if idy != N - 1:
				outfile.write(',\n\t\t')
		outfile.write('\n\t')
		if idx != number_of_topologies_to_simulate - 1:
			outfile.write('},\n\t')
	outfile.write('}\n};\n}\n')