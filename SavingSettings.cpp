#include <iostream>
#include <fstream>
#include <filesystem>
#include "SavingSettings.h"


void SavingSettings::save(SettingData levelData)
{
	std::ofstream MyFile;
	if (std::filesystem::exists(SavingSettings::path) == false)
	{
		MyFile.open(SavingSettings::path, 2);
	}
	MyFile.open(SavingSettings::path, 2);

	std::vector<std::string> csvData;
	
	std::string diffvalue             = "SnakeSpeed:" +      std::to_string(levelData.snakeSpeed) + "\n";
	std::string wallSpawnAmountvalue  = "WallSpawnAmount:" + std::to_string(levelData.wallSpawnAmount) + "\n";
	std::string WallColorvalue        = "WallColor:" +       std::to_string(levelData.wallColor) + "\n";
	std::string SnakeColorvalue       = "SnakeColor:" +      std::to_string(levelData.snakeColor) + "\n";
	std::string FoodColorvalue        = "FoodColor:" +       std::to_string(levelData.foodColor) + "\n";
	std::string ShowDebugvalue        = "ShowDebug:" +       std::to_string(levelData.showDebugs) + "\n";

	csvData.push_back(diffvalue);
	csvData.push_back(wallSpawnAmountvalue);
	csvData.push_back(WallColorvalue);
	csvData.push_back(SnakeColorvalue);
	csvData.push_back(FoodColorvalue);
	csvData.push_back(ShowDebugvalue);

	for (size_t i = 0; i < csvData.size(); i++)
	{
		MyFile << csvData[i];
	}
	MyFile.close();
}
SettingData SavingSettings::load()
{
	SettingData data;
	std::string output;
	std::vector<std::string> dataRows;

	std::ifstream MyReadFile(SavingSettings::path);

	while (std::getline(MyReadFile, output))
	{
		dataRows.push_back(output);
	}
	
	std::vector<std::vector <std::string>> allDataSegments;


	for (size_t i = 0; i < dataRows.size(); i++)
	{
		std::stringstream stringStream(dataRows[i]);
		std::vector <std::string> allCurrentSegments;
		std::string segment;

		while (std::getline(stringStream, segment, ':'))
		{
			allCurrentSegments.push_back(segment);
		}
		allDataSegments.push_back(allCurrentSegments);
		//levelData ld;
		////ld.level = std::remove(allSegments[0], ;
		//allSegments[0].erase(std::remove_if(allSegments[0].begin(), allSegments[0].end(), ::isspace),
		//	allSegments[0].end());



		//ld.level = allSegments[0];
		//ld.height = std::stoi(allSegments[1]);
		//ld.length = std::stoi(allSegments[2]);
		//ld.highScore = std::stoi(allSegments[3]);
		//data.push_back(ld);
	}
	data.snakeSpeed      = std::stoi(allDataSegments[0][1]);
	data.wallSpawnAmount = std::stoi(allDataSegments[1][1]);
	data.wallColor       = std::stoi(allDataSegments[2][1]);
	data.snakeColor      = std::stoi(allDataSegments[3][1]);
	data.foodColor       = std::stoi(allDataSegments[4][1]);
	data.showDebugs      = std::stoi(allDataSegments[5][1]);
	   
		
		
		
		
		

	return data;
}
