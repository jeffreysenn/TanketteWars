#pragma once
#include "ResourceManagers/ResourceIdentifiers.h"
#include "Map.h"

namespace mw
{
class CameraActor;
}

namespace sf
{
class RenderWindow;
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
	::sf::RenderWindow* window{};
	::mw::TextureManager* textureManager{};
	::mw::FontManager* fontManager{};
	::mw::SoundManager* soundManager{};
	::tankett::MapManager* mapManager{};
	class NetworkManager* networkManager{};
	bool isWindowFocused = true;
	::mw::CameraActor* camera{};
};
}