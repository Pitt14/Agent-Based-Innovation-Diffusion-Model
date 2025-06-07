#pragma once

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

class CompleteGraph
{
public:
	CompleteGraph()
	{
		_list_of_agents = std::vector <Agent> (ModelParameter::N);

		std::generate(_list_of_agents.begin(), _list_of_agents.end(), [this]()
		{
			return Agent(_random_number_handler.getBernoulliDistribution());
		});
	}

	void setAgentOpinion(int agent_position, bool new_opinion)
	{
		_list_of_agents[agent_position].setOpinion(new_opinion);
	}

	int getRandomAgent()
	{
		return _random_number_handler.getUniformIntDistribution();
	}

	float getRandomNumber()
	{
		return _random_number_handler.getUniformRealDistribution();
	}

	float getConcentrationOfAgents() const
	{
		int sum = std::accumulate(_list_of_agents.begin(), _list_of_agents.end(), 0);
		return static_cast <float> (sum) / static_cast <float> (ModelParameter::N);
	}

	// function that constructs the group of influence and checks the impact of the group
	GroupImpact checkImpactOfGroupOfInfluence(int agent_position, uint16_t q)
	{
		std::vector <std::size_t> random_samples = _random_number_handler.getRandomSampleWithException(ModelParameter::N, q, agent_position);
		
		std::size_t sum_of_opinions = 0;
		for(std::size_t sample : random_samples) {
			sum_of_opinions += _list_of_agents[sample].getOpinion();
		}

		// the whole group of influence is for 'no'
		if(sum_of_opinions == 0) {
			return GroupImpact::negative_impact;
		}

		// the whole group of influence is for 'yes'
		if(sum_of_opinions == random_samples.size()) {
			return GroupImpact::positive_impact;
		}

		// neighbors in the group of influence disagree
		return GroupImpact::neutral_impact;
	}

private:
	std::vector <Agent> _list_of_agents;

	RandomNumberHandler _random_number_handler;
};
