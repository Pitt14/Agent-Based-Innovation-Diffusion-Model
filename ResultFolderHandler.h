#pragma once

#include "ModelParameter.h"

#include <string>
#include <format>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

// class used to create folder for the results for a given set of parameters
// and to create output files and write into them single trajectories
class ResultFolderHandler
{
public:
	// method for creating a folder for the results for a given set of parameters
	ResultFolderHandler(uint16_t q, float p_eng, std::filesystem::path origin)
	{
		#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == COMPLETE_GRAPH
		std::string name = "Results_N_" + std::to_string(ModelParameter::N)
			+ "_q_" + std::to_string(q) + "_p_eng_" + std::format("{:.2f}", p_eng);
		#elif NETWORK_MODEL == THEORETICAL_GRAPH
		std::string name = "Results_WS_N_" + std::to_string(ModelParameter::N)
			+ "_k_" + std::to_string(ModelParameter::k_min)
			+ "_beta_" + std::format("{:.4f}", ModelParameter::b_min)
			+ "_q_" + std::to_string(q) + "_p_eng_" + std::format("{:.2f}", p_eng);
		#endif

		_folder_name = origin.append(name);

		std::cout << "Creating a folder: " << _folder_name << std::endl;

		// check if the folder exists
		if (!std::filesystem::exists(_folder_name))
		{
			if (!std::filesystem::create_directory(_folder_name))
			{
				std::cerr << "Error: cannot create a folder: " << _folder_name << std::endl;
				std::terminate();
			}
		}
		else
		{
			std::cerr << "Error: folder " << _folder_name << " already exists" << std::endl;
			std::terminate();
		}
	}

	ResultFolderHandler(ResultFolderHandler&) = delete;	// forbid to ever clone the instance of folder handler

	ResultFolderHandler(ResultFolderHandler&& other): _folder_name(other.getFolderName()) {}	// allow user to move the instance of folder handler

	// return the name of the folder
	std::filesystem::path getFolderName() const {
		return _folder_name;
	}

	// function to create output file and to write into the file single trajectory
	void writeResults(
	uint16_t q,
	float p_eng,
	float p_ind,
	int graph_number,
	const std::array <float, ModelParameter::T + 1>& concentration_for_yes)
	{
		#if NETWORK_MODEL == PREDEFINED_TOPOLOGY || NETWORK_MODEL == COMPLETE_GRAPH
		std::string file_name = "Traj_N_" + std::to_string(ModelParameter::N)
			+ "_q_" + std::to_string(q) + "_p_eng_" + std::format("{:.2f}", p_eng)
			+ "_p_ind_" + std::format("{:.2f}", p_ind) + "_graph_" + std::to_string(graph_number) + ".csv";
		#elif NETWORK_MODEL == THEORETICAL_GRAPH
		std::string file_name = "Traj_WS_N_" + std::to_string(ModelParameter::N)
			+ "_k_" + std::to_string(ModelParameter::k_min)
			+ "_beta_" + std::format("{:.4f}", ModelParameter::b_min)
			+ "_q_" + std::to_string(q) + "_p_eng_" + std::format("{:.2f}", p_eng)
			+ "_p_ind_" + std::format("{:.2f}", p_ind) + "_graph_" + std::to_string(graph_number) + ".csv";
		#endif

		// open the output file
		std::filesystem::path full_file_name = _folder_name;
		full_file_name = full_file_name.append(file_name);

		std::ofstream file(full_file_name);

		// checking if the output file was opened correctly
		if (!file.is_open())
		{
			std::cerr << "Error: cannot open the file: " << file_name << std::endl;
			return;
		}

		// writing subsequent values of the concentration_for_yes to a .csv file
		for (size_t i = 0; i < ModelParameter::T; i++)
		{
			file << concentration_for_yes[i];
			if (i != ModelParameter::T - 1)	// omit the last comma
			{
				file << ",";
			}
		}
	}

private:
	std::filesystem::path _folder_name;
};
