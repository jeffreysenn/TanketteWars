#pragma once
#include "ResourceManagers/ResourceIdentifiers.h"
#include "Map.h"

namespace mw
{
class CameraActor;
}

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
	::mw::TextureManager* textureManager{};
	::mw::FontManager* fontManager{};
	::tankett::MapManager* mapManager{};
	class NetworkManager* networkManager{};
	bool isWindowFocused = true;
	::mw::CameraActor* camera{};
};
}