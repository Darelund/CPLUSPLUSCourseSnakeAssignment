#include "MainMenu.h"
#include <conio.h>





void MainMenu::switchButton(char& lastKey, int& currentBtn, std::vector<button>& buttons)
{
    if (_kbhit)
    {
        switch (_getch())
        {
        case 'w':
            if (currentBtn > 0)
            {
                lastKey = 'W';
                currentBtn--;
            }
            break;
        case 's':
            if (currentBtn < buttons.size() - 1)
            {
                currentBtn++;
                lastKey = 'S';
            }
            break;
        }
    }
};
void MainMenu::ChooseLevel(gameData& gameData, int level)
{
    gameData.selectedLevel = std::to_string(level);
}
void MainMenu::init(gameData& gameData, std::vector<levelData> data)
{
#pragma region  startButtons
    std::vector<button> startButtons;

    button playBtn;
    playBtn.text.text = "Play";
    playBtn.text.pos.setPos(0, 0);
    playBtn.performCommand = [&]() {currentMenu = menu::Level; };

    button scoreBtn;
    scoreBtn.text.text = "HighScores";
    scoreBtn.text.pos.setPos(0, 1);
    scoreBtn.performCommand = [&]() {currentMenu = menu::Score; };

    button settingsBtn;
    settingsBtn.text.text = "Settings";
    settingsBtn.text.pos.setPos(0, 2);
    settingsBtn.performCommand = [&]() {system("setting.txt"); };

    button quitBtn;
    quitBtn.text.text = "Quit";
    quitBtn.text.pos.setPos(0, 3);
    quitBtn.performCommand = [&]() {inMenu = false; gameData.currentState = Quit; };

    startButtons.push_back(playBtn);
    startButtons.push_back(scoreBtn);
    startButtons.push_back(settingsBtn);
    startButtons.push_back(quitBtn);

    menuPanel startMenu;
    startMenu.InitMenuPanelButtons(startButtons);
#pragma endregion
#pragma region  highscoreButtons
    std::vector<button> scoreButtons;
    std::vector<text> scoreTexts;

    text highScoreText;
    highScoreText.text = "HIGHSCORES";
    highScoreText.pos.setPos(0, 0);
    highScoreText.normalColor = FOREGROUND_GREEN + FOREGROUND_BLUE;

    text level1HighScore;
    level1HighScore.text = "Level1: " + std::to_string(data[0].highScore);
    level1HighScore.pos.setPos(0, 1);

    text level2HighScore;
    level2HighScore.text = "Level2: " + std::to_string(data[1].highScore);
    level2HighScore.pos.setPos(0, 2);

    text level3HighScore;
    level3HighScore.text = "Level3: " + std::to_string(data[2].highScore);
    level3HighScore.pos.setPos(0, 3);

    button backBtn;
    backBtn.text.text = "Back";
    backBtn.text.pos.setPos(0, 4);
    backBtn.performCommand = [&]() {currentMenu = menu::Menu; };

    scoreTexts.push_back(highScoreText);
    scoreTexts.push_back(level1HighScore);
    scoreTexts.push_back(level2HighScore);
    scoreTexts.push_back(level3HighScore);

    scoreButtons.push_back(backBtn);


    menuPanel scoreMenu;
    scoreMenu.InitMenuPanelButtons(scoreButtons);
    scoreMenu.InitMenuPanelTexts(scoreTexts);
#pragma endregion

#pragma region  LevelButtons
    std::vector<button> levelButtons;

    button level1Btn;
    level1Btn.text.text = "Level1";
    level1Btn.text.pos.setPos(0, 0);
    level1Btn.performCommand = [&]() {ChooseLevel(gameData, 1); inMenu = false; };


    button level2Btn;
    level2Btn.text.text = "Level2";
    level2Btn.text.pos.setPos(0, 1);
    level2Btn.performCommand = [&]() {ChooseLevel(gameData, 2); inMenu = false; };


    button level3Btn;
    level3Btn.text.text = "Level3";
    level3Btn.text.pos.setPos(0, 2);
    level3Btn.performCommand = [&]() {ChooseLevel(gameData, 3); inMenu = false; };


    button levelBackBtn;
    levelBackBtn.text.text = "Back";
    levelBackBtn.text.pos.setPos(0, 3);
    levelBackBtn.performCommand = [&]() {currentMenu = menu::Menu; };


    levelButtons.push_back(level1Btn);
    levelButtons.push_back(level2Btn);
    levelButtons.push_back(level3Btn);
    levelButtons.push_back(levelBackBtn);

    menuPanel levelMenu;
    levelMenu.InitMenuPanelButtons(levelButtons);
#pragma endregion

    menus.insert({ menu::Menu, startMenu });
    menus.insert({ menu::Score, scoreMenu });
    menus.insert({ menu::Level, levelMenu });
}
void MainMenu::MainMenu::mainMenu()
{
    currentMenu = menu::Menu;
 
    lastKey = 'W';
    currentBtn = 0;
    inMenu = true;
    while (inMenu)
    {
        menuPanel menu = menus[currentMenu];
        menu.show(currentBtn);
        renderBuffer();
        switchButton(lastKey, currentBtn, menu.buttons);
        menu.useButton(currentBtn);
        clearBuffer();
    }
}