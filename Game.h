#pragma once
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <time.h>
#include <functional>
#include "console_renderer.h"
#include "saving.h"
#include "levelData.h"
#include "settingData.h"
#include "SavingSettings.h"
#include "UI.h"
#include "Position.h"
#include "inputData.h"
#include "gameData.h"
#include "MainMenu.h"
class Game
{
public:
	bool isGameRunning = true;


	bool collided(Position2 p, int type, map& map);
	bool canWalk(int y, int x, map& map);


	void updateRate(gameData& gameData);
	void DrawUI(gameData& gameData);
	void drawWorld(gameData& gameData);

	void draw(gameData& gameData);
	Position2 playerInput();
	void updateRandomTile(gameData& gameData, int type);
	void logic(gameData& gameData);
	void run();
};


