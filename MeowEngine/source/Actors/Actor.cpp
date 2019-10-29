#include "Actors/Actor.h"
#include "Commands/Command.h"
#include "Collisions/PhysicsEngine.h"

#include <algorithm>
#include <cassert>

namespace mw
{
Actor::Actor()
	: mParent(nullptr)
	, mCommandCategory(CommandCategory::None)
	, mPendingDestroy(false)
	, mRole(NetRole::None)
{
}

void Actor::attachChild(::std::unique_ptr<Actor> node)
{
	node->setParent(this);
	mChildren.push_back(::std::move(node));
}

::std::unique_ptr<Actor> Actor::detachChild(const Actor& node)
{
	auto found = ::std::find_if(mChildren.begin(), mChildren.end(),
								[&node](::std::unique_ptr<Actor>& ptr)
								{
									return ptr.get() == &node;
								});

	assert(found != mChildren.end());
	::std::unique_ptr<Actor> result = ::std::move(*found);
	result->resetParent();
	mChildren.erase(found);
	return ::std::move(result);
}

Actor* Actor::getSceneGraph()
{
	Actor* parent = this;
	while (parent->getParent() != nullptr)
	{
		parent = parent->getParent();
	}
	return parent;
}

void Actor::destroy(Actor* actor)
{
	actor->setPendingDestroy();
}

void Actor::update(float deltaSeconds)
{
	updateSelf(deltaSeconds);
	updateChildren(deltaSeconds);

	updateCollision(mOverlapPair, ::std::bind(&Actor::onOverlapExit, this));
}

void Actor::updateSelf(float deltaSeconds)
{
}

void Actor::updateChildren(float deltaSeconds)
{
	for (auto const& child : mChildren)
		child->update(deltaSeconds);
}

void Actor::reportRenderInfo(Renderer& renderer, ::sf::RenderStates states) const
{
	states.transform *= getTransform();

	reportRenderInfoSelf(renderer, states);
	reportRenderInfoChildren(renderer, states);
}

void Actor::reportRenderInfoSelf(Renderer& renderer, ::sf::RenderStates states) const
{
}

void Actor::reportRenderInfoChildren(Renderer& renderer, ::sf::RenderStates states) const
{
	for (auto const& child : mChildren)
		child->reportRenderInfo(renderer, states);
}

void Actor::reportCollisionInfo(PhysicsEngine& physicsEngine)
{
	reportCollisionInfoSelf(physicsEngine);
	reportCollisionInfoChildren(physicsEngine);
}

void Actor::reportCollisionInfoSelf(PhysicsEngine& physicsEngine)
{
	if (getCollider())
		physicsEngine.pushCollisionInfo(this, getCollider());
}

void Actor::reportCollisionInfoChildren(PhysicsEngine& physicsEngine)
{
	for (auto& child : mChildren)
	{
		child->reportCollisionInfo(physicsEngine);
	}
}

void Actor::updateCollision(::std::pair<bool, bool>& collisionPair, ::std::function<void(void)> exitFunction)
{
	if (collisionPair.first && !collisionPair.second)
		exitFunction();

	collisionPair.first = collisionPair.second;
	collisionPair.second = false;
}

void Actor::onCollision(Actor& other)
{
	onCollisionEnter(other);
}

void Actor::onCollisionEnter(Actor& other)
{
}

void Actor::onOverlap(Actor& other)
{
	onOverlapStay(other);
	mOverlapPair.second = true;
	if (!mOverlapPair.first)
		onOverlapEnter(other);
}

void Actor::onOverlapEnter(Actor& other)
{
}

void Actor::onOverlapExit()
{
}

void Actor::onOverlapStay(Actor& other)
{
}

void Actor::onCommand(const Command& command, float deltaSeconds)
{
	if ((command.category & mCommandCategory) != CommandCategory::None)
		command.action(*this, deltaSeconds);

	for (int i = 0; i < mChildren.size(); ++i)
		mChildren[i]->onCommand(command, deltaSeconds);
}

::sf::Transform Actor::getWorldTransform() const
{
	::sf::Transform transform;

	for (const Actor* node = this; node != nullptr; node = node->mParent)
		transform *= node->getTransform();

	return transform;
}

::sf::Vector2f Actor::getWorldPosition() const
{
	return getWorldTransform() * ::sf::Vector2f();
}

void Actor::setWorldPosition(const ::sf::Vector2f& pos)
{
	::sf::Transform inversedParentTransform = (getWorldTransform() * getTransform().getInverse()).getInverse();

	::sf::Transform localTransform = inversedParentTransform.translate(pos);

	setPosition(localTransform * ::sf::Vector2f());
}

void Actor::moveWorldSpace(::sf::Vector2f move)
{
	setWorldPosition(getWorldPosition() + move);
}

}
