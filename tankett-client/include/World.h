#pragma once
#include <SFML/System/NonCopyable.hpp>

#include "ResourceManagers/ResourceIdentifiers.h"
#include "Map.h"
#include "Actors/SceneGraph.h"
#include "Rendering/Renderer.h"
#include "Commands/CommandQueue.h"
#include "Collisions/PhysicsEngine.h"

#include <array>
#include <memory>

using namespace mw;

namespace tankett
{
class Tank;
class TankManager;
}

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

class World : private ::sf::NonCopyable
{
public:
	World();
	~World();

	void update(float deltaSeconds);

	void draw();

	CommandQueue& getCommandQueue() { return mCommandQueue; }
	::tankett::TankManager* getTankManager() { return mTankManager; }
	SceneGraph& getSceneGraph() { return mSceneGraph; }

private:
	void loadResources();
	void loadMaps();
	void loadTextures();
	void loadSFX();

	void buildScene();

private:
	::sf::RenderWindow& mWindow;
	Renderer mRenderer;
	::mw::CameraActor* mCamera;
	tankett::TankManager* mTankManager;

	TextureManager& mTextureManager;
	SoundManager& mSoundManager;
	tankett::MapManager& mMapManager;

	PhysicsEngine mPhysicsEngine;
	SceneGraph mSceneGraph;
	::sf::Vector2f mSpawnPosition = ::sf::Vector2f(0, -20);
	CommandQueue mCommandQueue;
};


}