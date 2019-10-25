// main.cc

#include "Game.h"

int main(int argc, char **argv)
{
	Game* game = Game::create();

	game->run();

	delete game;
	return 0;
}
