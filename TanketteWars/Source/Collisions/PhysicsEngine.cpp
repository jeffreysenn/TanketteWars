#include "PhysicsEngine.h"
#include "../Helpers/VectorHelper.h"

void PhysicsEngine::checkCollision()
{
	for (size_t i = 0; i < Collision::objectTypeCount; ++i)
	{
		for (size_t j = 0; j < mCollisionBuffer[i].size(); ++j)
		{
			// loop through object types
			for (size_t m = i; m < Collision::objectTypeCount; ++m)
			{
				// if response to the object type is not ignore, 
				// loop through the vector of collisions
				if (mCollisionBuffer[i][j].collider->response[type_cast(m)] != Collision::ResponseType::Ignore)
				{
					size_t n = 0;
					if (i == m)
					{
						if (j + 1 < mCollisionBuffer[m].size())
							n = j + 1;
						else
							continue;
					}
					for (n; n < mCollisionBuffer[m].size(); ++n)
					{
						sf::FloatRect rectA = getTransformedRect(mCollisionBuffer[i][j]);
						sf::FloatRect rectB = getTransformedRect(mCollisionBuffer[m][n]);
						sf::Vector2f penetration = getPenetration(rectA, rectB);
						if (penetration != sf::Vector2f())
						{
							switch (mCollisionBuffer[i][j].collider->response[type_cast(m)]
									& mCollisionBuffer[m][n].collider->response[type_cast(i)])
							{
							case Collision::ResponseType::Collision:
							{
								resolveCollision(mCollisionBuffer[i][j], mCollisionBuffer[m][n], penetration);
								Actor* actorA = mCollisionBuffer[i][j].actor;
								Actor* actorB = mCollisionBuffer[m][n].actor;
								actorA->onCollision(*actorB);
								actorB->onCollision(*actorA);
							} break;
							case Collision::ResponseType::Overlap:
								mCollisionBuffer[i][j].actor->onOverlap(*mCollisionBuffer[m][n].actor);
								mCollisionBuffer[m][n].actor->onOverlap(*mCollisionBuffer[i][j].actor);
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
	}

	clearCollisionBuffer();
}

void PhysicsEngine::pushCollisionInfo(Actor * actor, Collider * collider)
{
	CollisionInfo collisionInfo(actor, collider);
	mCollisionBuffer[static_cast<int>(collider->type)].push_back(collisionInfo);
}

void PhysicsEngine::clearCollisionBuffer()
{
	for (auto &collisionInfoVector : mCollisionBuffer)
	{
		collisionInfoVector.clear();
	}
}

void PhysicsEngine::resolveCollision(const CollisionInfo& infoA, const CollisionInfo& infoB, const sf::Vector2f& penetration)
{
	std::map<Collision::ObjectType, uint8_t> typeCount;
	typeCount[infoA.collider->type]++;
	typeCount[infoB.collider->type]++;

	// Static and dynamic objects colliding: 
	if (typeCount[Collision::ObjectType::Dynamic] == 1
		&& typeCount[Collision::ObjectType::Static] == 1)
	{
		CollisionInfo dynamicInfo;
		CollisionInfo staticInfo;
		sf::Vector2f penetrationS2D;

		if (infoA.collider->type == Collision::ObjectType::Dynamic)
		{
			dynamicInfo = infoA;
			staticInfo = infoB;
			penetrationS2D = -penetration;
		}
		else
		{
			dynamicInfo = infoB;
			staticInfo = infoA;
			penetrationS2D = penetration;
		}
		dynamicInfo.actor->moveWorldSpace(penetrationS2D);
		return;
	}

	// TODO: resolve two dynamic objects colliding
	if (typeCount[Collision::ObjectType::Dynamic] == 2)
	{
		return;
	}
}

sf::FloatRect PhysicsEngine::getTransformedRect(CollisionInfo collisionInfo)
{
	return collisionInfo.actor->
		getWorldTransform().transformRect(collisionInfo.collider->rect);
}

sf::Vector2f PhysicsEngine::getPenetration(const sf::FloatRect& rectA, const sf::FloatRect& rectB) const
{
	// calculate Minkowski difference
	sf::FloatRect minkowski;
	minkowski.left = rectA.left - (rectB.left + rectB.width);
	minkowski.top = rectA.top - (rectB.top + rectB.height);
	minkowski.width = rectA.width + rectB.width;
	minkowski.height = rectA.height + rectB.height;

	// origin is not within minkowski difference, no collision
	if (!(minkowski.left < 0 && minkowski.top < 0
		&& minkowski.left + minkowski.width > 0
		&& minkowski.top + minkowski.height > 0))
		return sf::Vector2f();

	const sf::Vector2f vecs[4] = {
		sf::Vector2f(minkowski.left, 0),
		sf::Vector2f(0, minkowski.top),
		sf::Vector2f(minkowski.left + minkowski.width, 0),
		sf::Vector2f(0, minkowski.top + minkowski.height),
	};

	sf::Vector2f penetration;
	float min = FLT_MAX;
	for (const auto& vec : vecs)
	{
		float length = VectorHelper::getLength(vec);
		if (length < min)
		{
			min = length;
			penetration = vec;
		}
	}
	return penetration;
}

PhysicsEngine::CollisionInfo::CollisionInfo()
	: actor(nullptr)
	, collider(nullptr)
{
}

PhysicsEngine::CollisionInfo::CollisionInfo(Actor * actor, Collider * collider)
	:actor(actor)
	,collider(collider)
{
}
