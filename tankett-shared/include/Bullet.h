#pragma once

#include "Actors/SpriteActor.h"
#include "Collisions/Collider.h"

namespace tankett
{
class PlayerController;

class Bullet : public ::mw::SpriteActor
{
public:
	Bullet(PlayerController* owner);
	Bullet(const ::sf::Texture& texture, PlayerController* owner);
	~Bullet();

	void resetOwner() { mOwner = nullptr; }

	void setID(uint8_t id) { mID = id; }
	uint8_t getID() const { return mID; }

protected:
	virtual void onCollisionEnter(Actor& other) override;

	void registerCollider();

private:
	PlayerController* mOwner;
	uint8_t mID;
};
}