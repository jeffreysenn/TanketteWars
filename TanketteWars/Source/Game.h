#pragma once

#include "Worlds/World.h"
#include "Debug/FPSMeter.h"
#include "States/StateStack.h"
#include "Controllers/PlayerController.h"
#include "ResourceManagers/ResourceIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void registerStates();

	void handleInputs();

	void update(float deltaSeconds);

	void render();

private:
	sf::RenderWindow mWindow;
	PlayerController mPlayerController;

	StateStack mStateStack;

	TextureManager mTextureManager;
	FontManager mFontManager;


	sf::Text mStatsText;
	FPSMeter mFPSMeter;

	bool mbPaused;
};

