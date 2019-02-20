#include "Game.h"
#include <iostream>
int main()
{
  SpaceInvadersGame game;
  if (!game.init())
  {
    return -1;
  }

  game.run();

  std::cout << "Exiting Game!" << std::endl;
  return 0;
}