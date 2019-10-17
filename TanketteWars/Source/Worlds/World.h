#pragma once
#include <SFML/System/NonCopyable.hpp>

#include "../ResourceManagers/ResourceIdentifiers.h"
#include "../Actors/Actor.h"
#include "../Actors/SceneGraph.h"
#include "../Rendering/Renderer.h"
#include "../Commands/CommandQueue.h"
#include "../Collisions/PhysicsEngine.h"

#include <array>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>


class World : private sf::NonCopyable
{
public:
	explicit World(const struct Context &context);
	~World();

	void update(float deltaSeconds);

	void draw();

	CommandQueue& getCommandQueue() { return mCommandQueue; }
private:
	void loadResources();
	void loadTextures();

	void buildScene();

private:
	sf::RenderWindow &mWindow;
	Renderer mRenderer;
	class CameraActor* mCamera;
	class Actor* mBackgroundNode;
	TextureManager &mTextureManager;
	SceneGraph mSceneGraph;
	class AvatarActor* mAvatarActor;
	sf::Vector2f mSpawnPosition = sf::Vector2f(0, -20);
	sf::Vector2f mScrollVelocity;
	CommandQueue mCommandQueue;
	PhysicsEngine mPhysicsEngine;
};

