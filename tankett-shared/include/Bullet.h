#pragma once

#include "Actors/SpriteActor.h"
#include "Collisions/Collider.h"

using namespace mw;

namespace tankett
{
class Bullet : public SpriteActor
{
public:
	Bullet(class Tank* owner);
	Bullet(const ::sf::Texture& texture, class Tank* owner);
	~Bullet();

	void resetOwner() { mOwner = nullptr; }

	void setID(uint8_t id) { mID = id; }
	uint8_t getID() const { return mID; }

protected:
	virtual void onCollisionEnter(Actor& other) override;

	void registerCollider();

private:
	class Tank* mOwner;
	uint8_t mID;
};
}