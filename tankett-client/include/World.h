#pragma once
#include <SFML/System/NonCopyable.hpp>

#include "ResourceManagers/ResourceIdentifiers.h"
#include "ResourceManagers/Map.h"
#include "Actors/SceneGraph.h"
#include "Rendering/Renderer.h"
#include "Commands/CommandQueue.h"
#include "Collisions/PhysicsEngine.h"

#include <array>
#include <memory>

using namespace mw;

namespace mw
{
	class CameraActor;
	class Tank;
}

namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	World();
	~World();

	void update(float deltaSeconds);

	void draw();

	CommandQueue& getCommandQueue() { return mCommandQueue; }
private:
	void loadResources();
	void loadMaps();
	void loadTextures();

	void buildScene();

private:
	sf::RenderWindow &mWindow;
	Renderer mRenderer;
	CameraActor* mCamera;
	Tank* mAvatar;

	TextureManager& mTextureManager;
	MapManager& mMapManager;

	SceneGraph mSceneGraph;
	sf::Vector2f mSpawnPosition = sf::Vector2f(0, -20);
	CommandQueue mCommandQueue;
	PhysicsEngine mPhysicsEngine;
};

