#pragma once
#include "Game.h"
#include "tankett_shared.h"
#include "NetworkManager.h"
#include "World.h"
#include "Debug/FPSMeter.h"
#include "PlayerController.h"
#include "ResourceManagers/ResourceIdentifiers.h"
#include "ClientStateStack.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace client
{

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
	::sf::RenderWindow mWindow;

	TextureManager mTextureManager;
	FontManager mFontManager;
	SoundManager mSoundManager;

	::tankett::MapManager mMapManager;

	ClientStateStack mStateStack;

	::sf::Text mStatsText;
	FPSMeter mFPSMeter;

	NetworkManager mNetworkManager;
};


}