#pragma once

#define OMP_PARALLELIZATION 0
#define EXECUTION_POLICY_PARALLELIZATION 1
// choose the parallel processing tool
#define PARALLEL_PROCESSING_TOOL OMP_PARALLELIZATION

#include "ModelParameter.h"
#include "ResultHandler.h"
#include "ResultFolderHandler.h"
#include "ComputeTrajectory.h"
#include "MathFunctions.h"
#include "Libraries/CppProgressBar/CppProgressBar.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <execution>

// class used to handle the order of the for loops over the simulation parameters
class Executionflow
{
public:
	Executionflow():
		_result_handler(), _progress_bar(ModelParameter::LS * ModelParameter::q_steps * ModelParameter::p_ind_steps * ModelParameter::p_eng_steps)
	{
	}

	void execute()
	{
		computeGroupsOfInfluence();	// initially call the function computeGroupsOfInfluence()
	}

private:
	ResultHandler _result_handler;

	ProgressBar _progress_bar;

	// function to calculate the subsequent values of the p_ind parameter - the probabilities of independence
	void computeProbabilitiesOfIndependence(ResultFolderHandler& result_folder_handler, uint16_t q, float p_eng, uint16_t graph_number)
	{
		std::array <float, ModelParameter::p_ind_steps> probabilities_of_independence;
		std::generate(probabilities_of_independence.begin(), probabilities_of_independence.end(), [n = 0]() mutable {
			return asProbability(n++ * ModelParameter::dp_ind + ModelParameter::p_ind_min);
		});

		#if PARALLEL_PROCESSING_TOOL == OMP_PARALLELIZATION
		#pragma omp parallel for	// enable parallel computations
		for (auto& p_ind : probabilities_of_independence)
		{
			computeTrajectory(result_folder_handler, q, p_eng, p_ind, graph_number);	// call the function computeTrajectoryCompleteGraph() for each q, p_eng, p_ind and graph_number values
			_progress_bar.tick();
		}
		#elif PARALLEL_PROCESSING_TOOL == EXECUTION_POLICY_PARALLELIZATION
		std::for_each(EXECUTION_POLICY, probabilities_of_independence.begin(), probabilities_of_independence.end(), [&result_folder_handler, q, p_eng, graph_number, this](auto& p_ind){
			computeTrajectory(result_folder_handler, q, p_eng, p_ind, graph_number);
			_progress_bar.tick();
		});
		#endif
	}

	// function to calculate the subsequent values of graph_number (from 0 to LS - 1)
	void computeAllSimulations(ResultFolderHandler& result_folder_handler, uint16_t q, float p_eng)
	{
		std::array <uint16_t, ModelParameter::LS> simulations;
		std::generate(simulations.begin(), simulations.end(), [n = 0]() mutable {return n++;});

		#if PARALLEL_PROCESSING_TOOL == OMP_PARALLELIZATION
		#pragma omp parallel for	// enable parallel computations
		for (auto& graph_number : simulations)
		{
			computeProbabilitiesOfIndependence(result_folder_handler, q, p_eng, graph_number);	// call the function computeProbabilitiesOfIndependence() for each q, p_eng and graph_number values
		}
		#elif PARALLEL_PROCESSING_TOOL == EXECUTION_POLICY_PARALLELIZATION
		std::for_each(EXECUTION_POLICY, simulations.begin(), simulations.end(), [this, &result_folder_handler, q, p_eng](uint16_t& graph_number){
			computeProbabilitiesOfIndependence(result_folder_handler, q, p_eng, graph_number);
		});
		#endif
	}

	// function to calculate the subsequent values of the p_eng parameter - the probabilities of engagement
	void computeProbabilitiesOfEngagement(uint16_t q)
	{

		std::array <float, ModelParameter::p_eng_steps> probabilities_of_engagement;
		std::generate(probabilities_of_engagement.begin(), probabilities_of_engagement.end(), [n = 0]() mutable {
			return asProbability(n++ * ModelParameter::dp_eng + ModelParameter::p_eng_min);
		});

		std::for_each(probabilities_of_engagement.begin(), probabilities_of_engagement.end(), [this, q](auto& p_eng){
			computeAllSimulations(_result_handler.createNewFolder(q, p_eng), q, p_eng);	// call the function computeAllSimulations() for each q and p_eng values
		});
	}

	// function to calculate the subsequent values of the q parameter - the sizes of the group of influence
	void computeGroupsOfInfluence()
	{
		std::array <uint16_t, ModelParameter::q_steps> groups_of_influence;
		std::generate(groups_of_influence.begin(), groups_of_influence.end(), [n = 0]() mutable {
			return (n++ * ModelParameter::dq + ModelParameter::q_min);
		});

		std::for_each(groups_of_influence.begin(), groups_of_influence.end(), [this](auto& q){
			computeProbabilitiesOfEngagement(q);	// call the function computeProbabilitiesOfEngagement() for each q value
		});
	};
};