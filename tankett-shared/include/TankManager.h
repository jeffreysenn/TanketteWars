#pragma once
#include "Actors\Actor.h"
#include <array>

namespace mw
{
class CameraActor;
}

namespace tankett
{
class PlayerController;
class Map;

class TankManager : public ::mw::Actor
{
public:
	TankManager(Map* map = nullptr, ::mw::CameraActor* camera = nullptr);

	void setTankTextures(int tankID, int textureID, ::sf::Texture* texture);

	void spawnTankRandom(uint8_t id, PlayerController* controller = nullptr, ::mw::NetRole netRole = ::mw::NetRole::Authority);

	void spawnTank(::sf::Vector2f position, uint8_t id,  PlayerController* controller = nullptr, ::mw::NetRole netRole = ::mw::NetRole::Authority);

private:
	::std::array<std::array<::sf::Texture*, 3>, 4> mTankTextures;
	Map* mMap;
	::mw::CameraActor* mCamera;
};
}