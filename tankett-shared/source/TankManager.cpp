#include "TankManager.h"
#include "Tank.h"
#include "Map.h"
#include "Unit.h"
#include "PlayerController.h"
#include "Actors/CameraActor.h"
#include <cmath>

namespace tankett
{
TankManager::TankManager(Map* map, ::mw::CameraActor* camera)
	: mTankTextures{0}
	, mMap(map)
	, mCamera(camera)
{
	setCommandCategory(::mw::CommandCategory::TankManager);
}

void TankManager::setTankTextures(int tankID, int textureID, ::sf::Texture* texture)
{
	mTankTextures[tankID][textureID] = texture;
}

void TankManager::spawnTankRandom(uint8_t id, PlayerController* controller, ::mw::NetRole netRole)
{
	if (!mMap)
		return;

	::sf::Vector2i randomTile = mMap->getRandomEmptyTile();
	sf::Vector2f position(randomTile.x * unit::unit2pix(1), randomTile.y * unit::unit2pix(1));
	spawnTank(position, id, controller, netRole);
}

void TankManager::spawnTank(::sf::Vector2f position, uint8_t id,  PlayerController* controller, ::mw::NetRole netRole)
{
	::sf::Texture* tankHullTexture = mTankTextures[id][0];
	::sf::Texture* tankBarrelTexture = mTankTextures[id][1];
	::sf::Texture* tankBulletTexture = mTankTextures[id][2];
	std::unique_ptr<Tank> tank;
	if (tankHullTexture && tankBarrelTexture && tankBulletTexture)
	{
		tank = std::make_unique<Tank>(*tankHullTexture, *tankBarrelTexture, *tankBulletTexture);
	}
	else
	{
		tank = std::make_unique<Tank>();
	}
	tank->setNetRole(netRole);
	tank->setPosition(position);

	// attach a camera if AutonomousProxy
	if (netRole == ::mw::NetRole::AutonomousProxy && mCamera)
	{
		tank->setCamera(mCamera);
		mCamera->attachToActor(tank.get());
	}

	if (controller)
	{
		controller->possessTank(tank.get());
	}

	attachChild(std::move(tank));
}
}