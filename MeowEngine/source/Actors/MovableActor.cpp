#include "Actors/MovableActor.h"

namespace mw
{

MovableActor::MovableActor()
{
}

::sf::Vector2f MovableActor::getWorldVelocity() const
{
	::sf::Vector2f worldVel = ::sf::Vector2f();
	for (const Actor* node = this; node != nullptr; node = node->getParent())
		if (const MovableActor* actor = dynamic_cast<const MovableActor*>(node))
			worldVel += actor->getVelocity();

	return worldVel;
}

void MovableActor::updateSelf(float deltaSeconds)
{
	move(mVelocity * deltaSeconds);
}

}