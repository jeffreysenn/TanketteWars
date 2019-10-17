#pragma once

#include "SpriteActor.h"
#include "../Collisions/Collider.h"

class Bullet : public SpriteActor
{
public:
	Bullet(const sf::Texture& texture, class Tank* owner = nullptr);
	~Bullet();

	void resetOwner() { mOwner = nullptr; } 

protected:
	virtual Collider* getCollider() override;
	virtual void onCollisionEnter(Actor& other) override;

private:
	Collider mCollider;
	class Tank* mOwner;
};