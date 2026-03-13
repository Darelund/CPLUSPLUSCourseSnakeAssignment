#include "Game.h"


int main()
{
    //Init data
    setupCustomConsole();
    std::srand(static_cast<unsigned int> (std::time(nullptr)));
    
    Game game;
    game.run();
  
    deleteCustomConsole();
    
}