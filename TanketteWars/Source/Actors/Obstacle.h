#include "SpriteActor.h"
#include "../Collisions/Collider.h"

class Obstacle : public SpriteActor
{
public:
	Obstacle(float sizeX, float sizeY, const sf::Texture& texture);

	virtual Collider* getCollider() override { return &mCollider; }

	virtual void onCollisionEnter(Actor& other) override { printf("WallCollision!\n"); }
private:
	Collider mCollider;
};