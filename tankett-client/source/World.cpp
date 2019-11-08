#include "World.h"
#include "Context.h"
#include "Unit.h"
#include "Actors/Actor.h"
#include "Actors/CameraActor.h"
#include "Tank.h"
#include "Actors/Obstacle.h"
#include "TankManager.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

using namespace tankett;
namespace client
{

World::World()
	: mWindow(*Context::getInstance().window)
	, mTextureManager(*Context::getInstance().textureManager)
	, mMapManager(*Context::getInstance().mapManager)
	, mSoundManager(*Context::getInstance().soundManager)
	, mCamera(nullptr)
	, mSceneGraph(&mPhysicsEngine)
{
	loadResources();
	buildScene();
}

World::~World()
{
}

void World::loadResources()
{
	loadMaps();
	loadTextures();
	loadSFX();
}

void World::loadMaps()
{
	mMapManager.load(Map::DefaultMap, "../Assets/Maps/DefaultMap.txt");
}

void World::loadTextures()
{
	mTextureManager.load(Texture::Grass, "../Assets/PNG/Environment/grass.png");
	mTextureManager.load(Texture::Dirt, "../Assets/PNG/Environment/dirt.png");

	mTextureManager.load(Texture::TankBlackHull, "../Assets/PNG/Tanks/tankBlack_outline.png");
	mTextureManager.load(Texture::TankBlackBarrel, "../Assets/PNG/Tanks/barrelBlack_outline.png");
	mTextureManager.load(Texture::TankBlackBullet, "../Assets/PNG/Bullets/bulletSilverSilver_outline.png");

	mTextureManager.load(Texture::TankRedHull, "../Assets/PNG/Tanks/tankRed_outline.png");
	mTextureManager.load(Texture::TankRedBarrel, "../Assets/PNG/Tanks/barrelRed_outline.png");
	mTextureManager.load(Texture::TankRedBullet, "../Assets/PNG/Bullets/bulletRedSilver_outline.png");
	
	mTextureManager.load(Texture::TankGreenHull, "../Assets/PNG/Tanks/tankGreen_outline.png");
	mTextureManager.load(Texture::TankGreenBarrel, "../Assets/PNG/Tanks/barrelGreen_outline.png");
	mTextureManager.load(Texture::TankGreenBullet, "../Assets/PNG/Bullets/bulletGreenSilver_outline.png");
	
	mTextureManager.load(Texture::TankBlueHull, "../Assets/PNG/Tanks/tankBlue_outline.png");
	mTextureManager.load(Texture::TankBlueBarrel, "../Assets/PNG/Tanks/barrelBlue_outline.png");
	mTextureManager.load(Texture::TankBlueBullet, "../Assets/PNG/Bullets/bulletBlueSilver_outline.png");
}

void World::loadSFX()
{
	mSoundManager.load(Sound::TankFire, "../Assets/SFX/TankFiring.wav");
}

void World::buildScene()
{
	/* this is the scene graph:
							sceneGraph
		 /        				|			              \
	  tanks               backgroundNode					camera
								|
						background sprite
	*/

	::sf::Texture* dirtTexture = mTextureManager.get(Texture::Dirt);
	Map* map = mMapManager.get(Map::DefaultMap);
	auto mapObj = map->buildMap(unit::unit2pix(1), unit::unit2pix(1), *dirtTexture);
	mSceneGraph.attachChild(std::move(mapObj));

	// Create a camera and attach to the sceneGraph
	auto cameraActor(std::make_unique<::mw::CameraActor>());
	mCamera = cameraActor.get();
	Context::getInstance().camera = mCamera;
	mSceneGraph.attachChild(std::move(cameraActor));

#define DYNAMIC_CAMERA
#ifdef DYNAMIC_CAMERA 
	// dynamic camera
	const ::sf::Vector2f cameraSize(1920, 1080);
	mCamera->setSize(cameraSize);
	auto tankManager = std::make_unique<TankManager>(map, mCamera, mSoundManager.get(Sound::TankFire));

#else 
	//static camera
	float ratio = map->getSize().y * unit::unit2pix(1)/9;
	const ::sf::Vector2f cameraSize(16*ratio, map->getSize().y * unit::unit2pix(1));
	mCamera->setPosition(cameraSize.x / 2-500, cameraSize.y / 2 - unit::unit2pix(.5f));
	auto tankManager = std::make_unique<TankManager>(map);
	mCamera->setSize(cameraSize);
#endif

	mTankManager = tankManager.get();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mTankManager->setTankTextures(i, j, mTextureManager.get((Texture::ID)(i * 3 + j + Texture::TankBlackHull)));
		}
	}

	mSceneGraph.attachChild(std::move(tankManager));

	// Create a background node and attach to the scene graph
	std::unique_ptr<Actor> backgroundNode(std::make_unique<Actor>());

	::sf::Texture* backgroundTexture = mTextureManager.get(Texture::Grass);
	backgroundTexture->setRepeated(true);
	std::unique_ptr<SpriteActor> backgroundSprite(std::make_unique<SpriteActor>(
		*backgroundTexture,
		Rendering::Layer::Background));
	backgroundSprite->getSprite()->setOrigin(0, 0);
	::sf::IntRect backgroundRect(0, 0, (int)1e4, (int)1e4);
	backgroundSprite->setSpriteRect(backgroundRect);
	backgroundNode->attachChild(std::move(backgroundSprite));

	mSceneGraph.attachChild(std::move(backgroundNode));
}

void World::update(float deltaSeconds)
{
	mSceneGraph.update(deltaSeconds);
	mSceneGraph.checkSceneCollision();
	mSceneGraph.enforceDestruction(mSceneGraph);
}

void World::draw()
{
	mWindow.setView(*mCamera);
	mSceneGraph.reportRenderInfo(mRenderer);
	mWindow.draw(mRenderer);
	mRenderer.clearRenderBuffer();
}
}