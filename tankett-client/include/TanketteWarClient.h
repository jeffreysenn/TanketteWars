#pragma once
#include "Game.h"
#include "tankett_shared.h"
#include "ClientNetworkManager.h"
#include "World.h"
#include "Debug/FPSMeter.h"
#include "Controllers/PlayerController.h"
#include "ResourceManagers/ResourceIdentifiers.h"
#include "ClientStateStack.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class TanketteWarClient : public Game
{
public:
	TanketteWarClient();
	void run();

private:
	void registerStates();

	void handleInputs();

	void update(float deltaSeconds);

	void render();

private:
	sf::RenderWindow mWindow;
	PlayerController mPlayerController;

	TextureManager mTextureManager;
	FontManager mFontManager;
	MapManager mMapManager;

	ClientStateStack mStateStack;

	sf::Text mStatsText;
	FPSMeter mFPSMeter;

	ClientNetworkManager mNetworkManager;
};

