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

	void setIsLocal(bool isLocal);


protected:
	virtual Collider* getCollider() override;
	virtual void onCollisionEnter(Actor& other) override;

private:
	Collider mCollider;
	class Tank* mOwner;
	uint8_t mID;
	bool mIsLocal;
};
}