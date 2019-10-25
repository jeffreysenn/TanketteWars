#pragma once
#include <SFML/System/NonCopyable.hpp>

#include "ResourceManagers/ResourceIdentifiers.h"
#include "Actors/Actor.h"
#include "Actors/SceneGraph.h"
#include "Rendering/Renderer.h"
#include "Commands/CommandQueue.h"
#include "Collisions/PhysicsEngine.h"

#include <array>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>


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
	class CameraActor* mCamera;
	class Actor* mBackgroundNode;
	TextureManager &mTextureManager;
	MapManager& mMapManager;
	SceneGraph mSceneGraph;
	class Tank* mAvatar;
	sf::Vector2f mSpawnPosition = sf::Vector2f(0, -20);
	CommandQueue mCommandQueue;
	PhysicsEngine mPhysicsEngine;
};

