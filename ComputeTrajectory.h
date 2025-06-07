#pragma once

#if NETWORK_MODEL == PREDEFINED_TOPOLOGY
#define NETWORK_TYPE SocialNetwork
#elif NETWORK_MODEL == COMPLETE_GRAPH
#define NETWORK_TYPE CompleteGraph
#elif NETWORK_MODEL == THEORETICAL_GRAPH
#define NETWORK_TYPE TheoreticalGraph
#endif

#include "ModelParameter.h"
#include "Graph.h"

#include <cstdint>
#include <string>
#include <iostream>

// function to handle independence submodel
#if NETWORK_MODEL == PREDEFINED_TOPOLOGY
void independence(int target_agent_index, SocialNetwork& network, float p_eng)
#elif NETWORK_MODEL == COMPLETE_GRAPH
void independence(int target_agent_index, CompleteGraph& network, float p_eng)
#elif NETWORK_MODEL == THEORETICAL_GRAPH
void independence(int target_agent_index, TheoreticalGraph& network, float p_eng)
#endif
{
	// with probability of engagement p_eng adapt the behavior
	if (network.getRandomNumber() < p_eng)
	{
		network.setAgentOpinion(target_agent_index, true);
		return;
	}
	
	// with complementary probability 1 - p_eng reject the behavior
	else
	{
		network.setAgentOpinion(target_agent_index, false);
		return;
	}
}

// function to handle conformity submodel
void conformity(int target_agent_index, NETWORK_TYPE& network, uint16_t q, int graph_number)
{
	// construct the q-panel without replacement and check the neighbors opinions
	#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == COMPLETE_GRAPH
	GroupImpact group_impact = network.checkImpactOfGroupOfInfluence(target_agent_index, q);
	#elif NETWORK_MODEL == THEORETICAL_GRAPH
	GroupImpact group_impact = network.checkImpactOfGroupOfInfluence(target_agent_index, q, graph_number);
	#endif

	// the whole group of influence is for 'yes'
	if (group_impact == GroupImpact::positive_impact)
	{
		network.setAgentOpinion(target_agent_index, true);
		return;
	}
	
	// the whole group of influence is for 'no'
	if (group_impact == GroupImpact::negative_impact)
	{
		network.setAgentOpinion(target_agent_index, false);
		return;
	}
}

// function that initializes a network, generates a single trajectory and writes it to an output file
void computeTrajectory(ResultFolderHandler& result_folder_handler, uint16_t q, float p_eng, float p_ind, int graph_number)
{
	// initializing a network of agents with their initial states
	NETWORK_TYPE network;

	// creating a list for subsequent time steps of the total concentration of agents with positive opinion
	std::array <float, ModelParameter::T + 1> concentration_for_yes;
	
	// the initial concentration of agents with positive opinion
	concentration_for_yes[0] = network.getConcentrationOfAgents();

	// time loop
	for (std::size_t i_t = 1; i_t <= ModelParameter::T; i_t++)
	{
		// loop through all agents
		for (std::size_t i_n = 0; i_n < ModelParameter::N; i_n++)
		{
			// randomly choose a target agent to reconsider its opinion
    		const int target_agent_index = network.getRandomAgent();

			// independence
			if (network.getRandomNumber() < p_ind)
			{
				independence(target_agent_index, network, p_eng);
			}	

			// conformity
			else
			{
				conformity(target_agent_index, network, q, graph_number);
			}
		}

		// the concentration of agents with positive opinion for t = i_t
		concentration_for_yes[i_t] = network.getConcentrationOfAgents();
	}

	result_folder_handler.writeResults(q, p_eng, p_ind, graph_number, concentration_for_yes);
}
