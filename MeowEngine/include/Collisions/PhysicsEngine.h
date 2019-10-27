#pragma once

#include "Actors/Actor.h"
#include "Collider.h"
#include "CollisionIdentifiers.h"

#include <array>
#include <vector>

namespace mw
{

class PhysicsEngine
{
public:
	void checkCollision();

	void pushCollisionInfo(Actor* actor, Collider* collider);

private:
	struct CollisionInfo
	{
		CollisionInfo();
		CollisionInfo(Actor* actor, Collider* collider);
		Actor* actor;
		Collider* collider;
	};

	void clearCollisionBuffer();

	void resolveCollision(const CollisionInfo& infoA, const CollisionInfo& infoB, const ::sf::Vector2f& penetration);

	::sf::FloatRect getTransformedRect(CollisionInfo collisionInfo);

	// returns a penetration vector from rect A to B, returns 0 vector when no penetration
	::sf::Vector2f getPenetration(const ::sf::FloatRect& rectA, const ::sf::FloatRect& rectB) const;

private:
#define type_cast static_cast<Collision::ObjectType>

	::std::array<::std::vector<CollisionInfo>, Collision::objectTypeCount> mCollisionBuffer;
};


}