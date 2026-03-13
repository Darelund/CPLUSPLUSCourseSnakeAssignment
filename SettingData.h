#pragma once
#include <string>

class SettingData
{
public:
	int snakeSpeed = 1;
	int wallSpawnAmount = 1;
	unsigned short wallColor = 0;
	unsigned short snakeColor = 0;
	unsigned short foodColor = 0;
	bool showDebugs = false;
};