#pragma once

#include "ModelParameter.h"
#include "ResultFolderHandler.h"

#include <filesystem>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>

// class used to create the main folder for the results of simulations started on the current date and time
// and used to manage nested folder creation
class ResultHandler
{
public:
	ResultHandler()
	{
		if (!std::filesystem::exists("Results"))
		{
			std::filesystem::create_directory("Results");
		}

		std::stringstream current_date;
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);
		current_date << std::put_time(std::localtime(&time), "%Y-%m-%d %H-%M");	// create the full name of the folder
		_result_folder_name = std::filesystem::path("Results").append(current_date.str());

		// check if the folder already exists
		if (!std::filesystem::exists(_result_folder_name))
		{
			std::filesystem::create_directory(_result_folder_name);
		}
	}

	// function to create new folder instance inside of the result folder
	ResultFolderHandler& createNewFolder(uint16_t q, float p_eng)
	{
		return _result_folder_handlers.emplace_back(q, p_eng, _result_folder_name);
	}

private:
	std::filesystem::path _result_folder_name;
	std::vector <ResultFolderHandler> _result_folder_handlers;
};