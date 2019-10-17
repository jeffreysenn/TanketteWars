#include "World.h"

#include "../States/Context.h"

#include "../Actors/CameraActor.h"
#include "../Actors/AvatarActor.h"
#include "../Actors/Tank.h"
#include "../Actors/Obstacle.h"

#include "../Units/Units.h"

#include <SFML/Graphics/Rect.hpp>

#include <iostream>

World::World(const Context &context)
	: mWindow(*context.window)
	, mTextureManager(*context.textureManager)
	, mCamera(nullptr)
	, mAvatarActor(nullptr)
	, mScrollVelocity(30.f, 0)
{
	loadResources();
	buildScene();
}

World::~World()
{
	mTextureManager.unload(Texture::Middle);
	mTextureManager.unload(Texture::Avatar);
}

void World::loadResources()
{
	loadTextures();
}

void World::loadTextures()
{
	mTextureManager.load(Texture::Middle, "../Assets/sunny-land-files/environment/middle.png");
	mTextureManager.load(Texture::Avatar, "../Assets/sunny-land-files/spritesheets/player-idle.png");
	mTextureManager.load(Texture::TankBlackHull, "../Assets/PNG/Tanks/tankBlack_outline.png");
	mTextureManager.load(Texture::TankBlackBarrel, "../Assets/PNG/Tanks/barrelBlack_outline.png");
	mTextureManager.load(Texture::TankBlackBullet, "../Assets/PNG/Bullets/bulletSilverSilver_outline.png");
	mTextureManager.load(Texture::Oil, "../Assets/PNG/Obstacles/oil.png");
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

	for(int i=0; i < 4; ++i)
	{
		sf::Texture* tankHullTexture = mTextureManager.get(Texture::TankBlackHull);
		sf::Texture* tankBarrelTexture = mTextureManager.get(Texture::TankBlackBarrel);
		sf::Texture* tankBulletTexture = mTextureManager.get(Texture::TankBlackBullet);
		std::unique_ptr<Tank> tankActor = std::make_unique<Tank>(*tankHullTexture, *tankBarrelTexture, *tankBulletTexture);
		tankActor->setPosition(200, 150.f * i);
		tankActor->setCommandCategory((CommandCategory)((int)CommandCategory::Tank0 << i));
		mSceneGraph.attachChild(std::move(tankActor));
	}

	sf::Texture* oilTexture = mTextureManager.get(Texture::Oil);
	std::unique_ptr<Obstacle> wall = std::make_unique<Obstacle>(Units::unit2pix(1), Units::unit2pix(1), *oilTexture);
	wall->setPosition(500, 500);
	mSceneGraph.attachChild(std::move(wall));

	// Create a camera and attach to the scene graph
	std::unique_ptr<CameraActor> cameraActor(std::make_unique<CameraActor>());
	mCamera = cameraActor.get();
	mSceneGraph.attachChild(std::move(cameraActor));
	const sf::Vector2f cameraSize(1280, 720);
	mCamera->setSize(cameraSize);
	mCamera->setPosition(cameraSize.x / 2, cameraSize.y / 2);

	// Create a background node and attach to the scene graph
	std::unique_ptr<Actor> backgroundNode(std::make_unique<Actor>());
	mBackgroundNode = backgroundNode.get();
	mSceneGraph.attachChild(std::move(backgroundNode));

	sf::Texture* backgroundTexture = mTextureManager.get(Texture::Back);
	backgroundTexture->setRepeated(true);
	std::unique_ptr<SpriteActor> backgroundSprite(std::make_unique<SpriteActor>(
		*backgroundTexture, 
		Rendering::Layer::Background));
	backgroundSprite->getSprite()->setOrigin(0, 0);
	sf::IntRect backgroundRect(0, 0, (int)cameraSize.x, (int)cameraSize.y);
	backgroundSprite->getSprite()->setTextureRect(backgroundRect);
	mBackgroundNode->attachChild(std::move(backgroundSprite));
}

void World::update(float deltaSeconds)
{
	mWindow.setView(*mCamera);

	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), deltaSeconds);

	mSceneGraph.update(deltaSeconds);
	mSceneGraph.reportCollisionInfo(mPhysicsEngine);
	mPhysicsEngine.checkCollision();
	mSceneGraph.enforceDestruction(mSceneGraph);
}

void World::draw()
{
	mWindow.setView(*mCamera);
	mSceneGraph.reportRenderInfo(mRenderer);
	mWindow.draw(mRenderer);
	mRenderer.clearRenderBuffer();
}