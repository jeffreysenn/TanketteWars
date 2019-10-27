#pragma once
#include "ResourceManagers/ResourceIdentifiers.h"
#include "Map.h"

using namespace mw;
using namespace tankett;

namespace client
{
struct Context
{
public:
	static Context& getInstance()
	{
		static Context instance;
		return instance;
	}

private:
	Context() {}

public:
	Context(Context const&) = delete;
	void operator=(Context const&) = delete;

public:
	class ClientStateStack* stack{};
	class ::sf::RenderWindow* window{};
	TextureManager* textureManager{};
	FontManager* fontManager{};
	MapManager* mapManager{};
	class NetworkManager* networkManager{};
	bool isWindowFocused = true;
};
}