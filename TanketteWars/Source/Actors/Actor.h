#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "../Commands/CommandCategory.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <memory>
#include <vector>
#include <functional>

class Actor
	: public sf::Transformable, private sf::NonCopyable
{
public:
	Actor();

	void attachChild(std::unique_ptr<Actor> node);
	std::unique_ptr<Actor> detachChild(const Actor& node);

	void setParent(Actor* parent) { mParent = parent; }
	const Actor* getParent() const { return mParent; }
	Actor* getParent() { return mParent; }
	Actor* getSceneGraph();
	void resetParent() { mParent = nullptr; }

	std::vector<std::unique_ptr<Actor>>& getChildren() { return mChildren; }

	static void destroy(Actor* actor);
	void setPendingDestroy(bool shouldDestroy = true) { mPendingDestroy = shouldDestroy; }
	bool isPendingDestroy() { return mPendingDestroy; }

	void update(float deltaSeconds);

	void onCommand(const struct Command &command, float deltaSeconds);

	void reportRenderInfo(class Renderer &renderer, sf::RenderStates states = sf::RenderStates::Default) const;

	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;
	void setWorldPosition(const sf::Vector2f& pos);
	virtual sf::Vector2f getVelocity() const { return sf::Vector2f(); }
	virtual sf::Vector2f getWorldVelocity() const { return sf::Vector2f(); }
	void moveWorldSpace(sf::Vector2f move);

	void setCommandCategory(const CommandCategory &category) { mCommandCategory = category; }
	CommandCategory getCommandCategory() const { return mCommandCategory; }

	virtual struct Collider* getCollider() { return nullptr; }

	void reportCollisionInfo(class PhysicsEngine &physicsEngine);

	void onCollision(Actor &other);
	void onOverlap(Actor &other);

protected:
	virtual void updateSelf(float deltaSeconds);

	virtual void reportRenderInfoSelf(class Renderer &renderer, sf::RenderStates states) const;

	virtual void reportCollisionInfoSelf(class PhysicsEngine &physicsEngine);

	virtual void onCollisionEnter(Actor &other);
	// TODO: implement "other"
	virtual void onCollisionExit();
	virtual void onCollisionStay(Actor &other);

	virtual void onOverlapEnter(Actor &other);
	virtual void onOverlapExit();
	virtual void onOverlapStay(Actor &other);

private:
	void updateChildren(float deltaSeconds);

	void reportRenderInfoChildren(class Renderer &renderer, sf::RenderStates states) const;

	void reportCollisionInfoChildren(class PhysicsEngine &physicsEngine);

	void updateCollision(std::pair<bool, bool> &collisionPair, std::function<void(void)> exitFunction);

protected:
	CommandCategory mCommandCategory;

	std::vector<std::unique_ptr<Actor>> mChildren;

	Actor* mParent;

private:
	std::pair<bool, bool> mCollisionPair;
	std::pair<bool, bool> mOverlapPair;

	bool mPendingDestroy;
};

