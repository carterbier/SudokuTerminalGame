#include "Game.h"
#include "Terminal.h"

#include <iostream>

int main()
{
	Terminal terminal;
	if (!terminal.isReady())
	{
		std::cerr << "\nUnable to initialize terminal display.\n";
		return 1;
	}
		
	Game game;
	game.run();

	return 0;
}