#pragma once
#include <string>
#include "SettingData.h"

class SavingSettings
{
public:
	std::string path = "setting.txt";
	void save(SettingData settingData);
	SettingData load();
};