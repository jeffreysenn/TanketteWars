// main.cc

#include "Game.h"

int main(int argc, char **argv)
{
	mw::Game* game = mw::Game::create();

	game->run();

	delete game;
	return 0;
}
