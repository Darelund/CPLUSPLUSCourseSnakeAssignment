#include <iostream>
#include <fstream>
#include <filesystem>
#include "saving.h"


void SavingSystem::save(std::vector<levelData> levelData)
{

	std::ofstream MyFile;
	if (std::filesystem::exists(SavingSystem::path) == false)
	{

		MyFile.open(SavingSystem::path, 2);
	}
	//MyFile.open(SavingSystem::path, std::fstream::app);
	MyFile.open(SavingSystem::path, 2);
	std::vector<std::string> csvData;
	for (size_t i = 0; i < levelData.size(); i++)
	{
		std::string value = levelData[i].level 
			+ " " + std::to_string(levelData[i].height) 
			+ " " + std::to_string(levelData[i].length)
			+ " " + std::to_string(levelData[i].highScore);
		if ((i + 1) < levelData.size())
		{
			value += ",";
		}
		
		csvData.push_back(value);
	}

	
	for (size_t i = 0; i < csvData.size(); i++)
	{
		MyFile << csvData[i];
	}
	MyFile.close();
}
std::vector<levelData> SavingSystem::load()
{
	std::vector <levelData> data;
	std::string output;
	std::vector<std::string> dataRows;

	std::ifstream MyReadFile(SavingSystem::path);

	while (std::getline(MyReadFile, output, ','))
	{
		dataRows.push_back(output);
	}
	for (size_t i = 0; i < dataRows.size(); i++)
	{
		std::stringstream stringStream(dataRows[i]);
		std::vector <std::string> allSegments;
		std::string segment;

		while (std::getline(stringStream, segment, ' '))
		{
			allSegments.push_back(segment);
		}
		levelData ld;
		//ld.level = std::remove(allSegments[0], ;
		allSegments[0].erase(std::remove_if(allSegments[0].begin(), allSegments[0].end(), ::isspace),
			allSegments[0].end());



		ld.level = allSegments[0];
		ld.height = std::stoi(allSegments[1]);
		ld.length = std::stoi(allSegments[2]);
		ld.highScore = std::stoi(allSegments[3]);
		data.push_back(ld);
	}
	return data;
}
