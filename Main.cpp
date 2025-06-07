#include "ModelParameter.h"
#include "Executionflow.h"

#include <algorithm>
#include <vector>
#include <exception>

int main()
{
	#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == THEORETICAL_GRAPH
	// create an array of indexes from 0 to (MAX_NEIGHBORS - 1) to draw elements from the list of neighbors
	for(std::size_t i = 0; i < NetworkTopology::MAX_NEIGHBORS; i++) {
		ModelParameter::CONSECUTIVE_NUMBERS[i] = i;
	}
	#endif

	Executionflow executionflow;

	try {
		executionflow.execute();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	} catch (...) {
		std::cerr << "Unknown Error" << std::endl;
	}
}
