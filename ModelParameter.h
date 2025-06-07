#pragma once

// parameters of the network topology
#define PREDEFINED_TOPOLOGY 1
#define COMPLETE_GRAPH 2
#define THEORETICAL_GRAPH 3

#define NETWORK_MODEL COMPLETE_GRAPH // choose the topology for simulations

#if NETWORK_MODEL == PREDEFINED_TOPOLOGY
	#include "AutomaticallyGeneratedNetworkTopology.h"

#elif NETWORK_MODEL == THEORETICAL_GRAPH
	#include "AutomaticallyGeneratedNetworkTopologies.h"
#endif

#include <cstdint>
#include <execution>
#include <cstddef>
#include <iterator>
#include <array>
#include <type_traits>

namespace ModelParameter
{

// parameters of the model
#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == THEORETICAL_GRAPH
	constexpr std::size_t N = NetworkTopology::NETWORK_SIZE;	// number of agents in the network
#elif NETWORK_MODEL == COMPLETE_GRAPH
	constexpr std::size_t N = 1000;	// number of agents in the network
#endif

constexpr uint16_t q_min = 4;		// minimal size of the group of influence for conformists (usually up to a dozen, so we use unsigned short)
constexpr uint16_t q_steps = 1;		// number of different values of the q parameter
constexpr uint16_t dq = 0;			// delta used to calculate subsequent values of the q parameter

constexpr float p_eng_min = 0.75;	// minimal value of the probability of engagement for independent agents
constexpr uint16_t p_eng_steps = 1;	// number of different values of the p_eng parameter
constexpr float dp_eng = 0;			// delta used to calculate subsequent values of the p_eng parameter

constexpr float p_ind_min = 0;		// minimal value of the probability of acting independently in a single update
constexpr uint16_t p_ind_steps = 51;// number of different values of the p_ind parameter
constexpr float dp_ind = 0.02;		// delta used to calculate subsequent values of the p_ind parameter

// parameters of the Monte Carlo Simulations
constexpr float c0 = 0;				// initial concentration of agents with positive opinion (agents for "yes")
constexpr uint16_t T = 2000;			// simulation time (usually less than 2000, so we use unsigned short)
#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == COMPLETE_GRAPH
	constexpr uint16_t LS = 10;			// number of independent runs (usually less than 100, so we use unsigned short)
#elif NETWORK_MODEL == THEORETICAL_GRAPH
	constexpr uint16_t LS = NetworkTopology::NUMBER_OF_TOPOLOGIES_TO_SIMULATE;
#endif

// parameters of the Watts-Strogatz network
constexpr uint16_t k_min = 8;		// minimal value of the mean number of neighbors in the Watts-Strogatz network (usually up to 150, so we use unsigned short)

constexpr float b_min = 0.05;			// minimal value of the probability of rewiring an edge in the Watts-Strogatz network

#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == THEORETICAL_GRAPH
	std::array <std::size_t, NetworkTopology::MAX_NEIGHBORS> CONSECUTIVE_NUMBERS;	// indexes from 0 to (MAX_NEIGHBORS - 1) to draw elements from the list of neighbors
#endif

#define EXECUTION_POLICY std::execution::par	// enable parallel calculations
}