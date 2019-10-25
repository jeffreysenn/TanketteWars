#pragma once
#include "ResourceManagers/ResourceIdentifiers.h"

using namespace mw;
struct ClientContext
{
public:
	static ClientContext& getInstance()
	{
		static ClientContext instance;
		return instance;
	}

private:
	ClientContext() {}                 

public:
	ClientContext(ClientContext const&) = delete;
	void operator=(ClientContext const&) = delete;

public:
	class ClientStateStack* stack{};
	class sf::RenderWindow* window{};
	TextureManager* textureManager{};
	FontManager* fontManager{};
	MapManager* mapManager{};
	class PlayerController* playerController{};
	class ClientNetworkManager* networkManager{};
};