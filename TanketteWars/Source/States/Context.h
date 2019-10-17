#pragma once

#include "../ResourceManagers/ResourceIdentifiers.h"
#include "../Controllers/PlayerController.h"

#include <SFML/Graphics/RenderWindow.hpp>

struct Context
{
	explicit Context(sf::RenderWindow& window, TextureManager& textureManager,
					 FontManager& fontManager, PlayerController& playerController)
		: window(&window)
		, textureManager(&textureManager)
		, fontManager(&fontManager)
		, playerController(&playerController)
	{
	}

	sf::RenderWindow* window;
	TextureManager* textureManager;
	FontManager* fontManager;
	PlayerController* playerController;
};