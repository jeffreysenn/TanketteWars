#pragma once

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Commands/CommandCategory.h"
#include "Network/NetRole.h"
#include "Collisions/Collider.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <memory>
#include <vector>
#include <functional>

namespace mw
{
class PhysicsEngine;
class Renderer;

class Actor
	: public ::sf::Transformable, private ::sf::NonCopyable
{
public:
	Actor();

	void attachChild(::std::unique_ptr<Actor> node);
	::std::unique_ptr<Actor> detachChild(const Actor& node);

	void setParent(Actor* parent) { mParent = parent; }
	const Actor* getParent() const { return mParent; }
	Actor* getParent() { return mParent; }
	Actor* getSceneGraph();
	void resetParent() { mParent = nullptr; }

	::std::vector<::std::unique_ptr<Actor>>& getChildren() { return mChildren; }

	static void destroy(Actor* actor);
	void setPendingDestroy(bool shouldDestroy = true) { mPendingDestroy = shouldDestroy; }
	bool isPendingDestroy() { return mPendingDestroy; }

	void update(float deltaSeconds);

	void onCommand(const struct Command& command, float deltaSeconds);

	void reportRenderInfo(Renderer& renderer, ::sf::RenderStates states = ::sf::RenderStates::Default) const;

	::sf::Transform getWorldTransform() const;
	::sf::Vector2f getWorldPosition() const;
	void setWorldPosition(const ::sf::Vector2f& pos);
	virtual ::sf::Vector2f getVelocity() const { return ::sf::Vector2f(); }
	virtual ::sf::Vector2f getWorldVelocity() const { return ::sf::Vector2f(); }
	void moveWorldSpace(::sf::Vector2f move);

	void setCommandCategory(const CommandCategory& category) { mCommandCategory = category; }
	CommandCategory getCommandCategory() const { return mCommandCategory; }

	struct Collider* getCollider();

	void reportCollisionInfo(PhysicsEngine& physicsEngine);

	void onCollision(Actor& other);
	void onOverlap(Actor& other);

	void setNetRole(NetRole netRole) { mNetRole = netRole; }
	NetRole getNetRole() const { return mNetRole; }

protected:
	virtual void updateSelf(float deltaSeconds);

	virtual void reportRenderInfoSelf(Renderer& renderer, ::sf::RenderStates states) const;

	virtual void reportCollisionInfoSelf(PhysicsEngine& physicsEngine);

	virtual void onCollisionEnter(Actor& other);

	virtual void onOverlapEnter(Actor& other);
	// TODO: implement "other"
	virtual void onOverlapExit();
	virtual void onOverlapStay(Actor& other);

	void setCollider(::std::unique_ptr<Collider>& collider) { mCollider = std::move(collider); }

private:
	void updateChildren(float deltaSeconds);

	void reportRenderInfoChildren(Renderer& renderer, ::sf::RenderStates states) const;

	void reportCollisionInfoChildren(PhysicsEngine& physicsEngine);

	void updateCollision(::std::pair<bool, bool>& collisionPair, ::std::function<void(void)> exitFunction);

protected:
	CommandCategory mCommandCategory;
	::std::vector<::std::unique_ptr<Actor>> mChildren;
	Actor* mParent;

private:
	::std::unique_ptr<Collider> mCollider;
	::std::pair<bool, bool> mOverlapPair;
	bool mPendingDestroy;
	NetRole mNetRole;
};

}
