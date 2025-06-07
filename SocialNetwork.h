#pragma once

#include "AutomaticallyGeneratedNetworkTopology.h"
#include "ModelParameter.h"
#include "Agent.h"
#include "RandomNumberHandler.h"

#include <vector>
#include <algorithm>
#include <stdexcept>

// class used to handle the impact of the group of influence
enum class GroupImpact
{
	positive_impact,	// the group influences for 'yes'
	negative_impact,	// the group influences for 'no'
	neutral_impact		// the group has no influence
};

// class used to handle neighbors in the system, the social network (connections between agents) and the group of influence
class SocialNetwork
{
public:
	SocialNetwork()
	{
		std::generate(_list_of_agents.begin(), _list_of_agents.end(), [this]()
		{
			return Agent(_random_number_handler.getBernoulliDistribution());	// create agents with their initial opinion
		});
	}

	// function that changes the opinion of the agent
	void setAgentOpinion(int agent_position, bool new_opinion)
	{
		_list_of_agents[agent_position].setOpinion(new_opinion);
	}

	// function that chooses randomly a target agent - draws intiger from 0 to N-1
	int getRandomAgent()
	{
		return _random_number_handler.getUniformIntDistribution();
	}

	// function that draws real random number from 0 to 1
	float getRandomNumber()
	{
		return _random_number_handler.getUniformRealDistribution();
	}

	// function that calculates concentration of agents for 'yes'
	float getConcentrationOfAgents() const
	{
		int sum = std::accumulate(_list_of_agents.begin(), _list_of_agents.end(), 0);
		return static_cast <float> (sum) / static_cast <float> (ModelParameter::N);
	}

	// function that constructs the group of influence and checks the impact of the group
	GroupImpact checkImpactOfGroupOfInfluence(int agent_position, uint16_t q)
	{
		std::vector <std::size_t> random_samples = _random_number_handler.getRandomSample(NetworkTopology::NumberOfNeighbors[agent_position], q);
		
		std::size_t sum_of_opinions = 0;
		for(std::size_t sample : random_samples) {
			sum_of_opinions += _list_of_agents[NetworkTopology::Network[agent_position][sample]].getOpinion();
		}
		// if the target agent has less than q neighbors, always all the neighbors are in the group of influence
		std::size_t expected_value = std::min(NetworkTopology::NumberOfNeighbors[agent_position], static_cast<std::size_t>(q));

		// the whole group of influence is for 'no'
		if(sum_of_opinions == 0) {
			return GroupImpact::negative_impact;
		}

		// the whole group of influence is for 'yes'
		if(sum_of_opinions == expected_value) {
			return GroupImpact::positive_impact;
		}

		// neighbors in the group of influence disagree
		return GroupImpact::neutral_impact;
	}

private:
	std::array <Agent, ModelParameter::N> _list_of_agents;

	RandomNumberHandler _random_number_handler;
};
