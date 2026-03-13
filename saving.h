#pragma once
#include <string>
#include "levelData.h"

class SavingSystem
{
public:
	std::string path = "saving.txt";
	void save(std::vector<levelData> levelData);
	std::vector<levelData> load();
};