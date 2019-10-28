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

void TankManager::spawnTankRandom(::mw::CommandCategory category, bool isLocal, PlayerController* controller)
{
	if (!mMap)
		return;

	::sf::Vector2i randomTile = mMap->getRandomEmptyTile();
	sf::Vector2f position(randomTile.x * unit::unit2pix(1), randomTile.y * unit::unit2pix(1));
	spawnTank(position, category, isLocal, controller);
}

void TankManager::spawnTank(::sf::Vector2f position, ::mw::CommandCategory category, bool isLocal, PlayerController* controller)
{
	int tankIndex = (int)log2((uint32_t)category / (uint32_t)::mw::CommandCategory::Tank0);
	::sf::Texture* tankHullTexture = mTankTextures[tankIndex][0];
	::sf::Texture* tankBarrelTexture = mTankTextures[tankIndex][1];
	::sf::Texture* tankBulletTexture = mTankTextures[tankIndex][2];
	std::unique_ptr<Tank> tank;
	if (tankHullTexture && tankBarrelTexture && tankBulletTexture)
	{
		tank = std::make_unique<Tank>(*tankHullTexture, *tankBarrelTexture, *tankBulletTexture);
	}
	else
	{
		tank = std::make_unique<Tank>();
	}
	tank->setCommandCategory(category);
	tank->setPosition(position);
	tank->setIsLocal(isLocal);
	if (mCamera && isLocal)
	{
		tank->setCamera(mCamera);
		mCamera->attachToActor(tank.get());
	}
	if (controller)
	{
		tank->setController(controller);
		controller->possessTank(tank.get());
	}
	attachChild(std::move(tank));
}
}