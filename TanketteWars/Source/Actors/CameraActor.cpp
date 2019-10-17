#include "CameraActor.h"

CameraActor::CameraActor()
	: MovableActor()
{
}


CameraActor::~CameraActor()
{
}

void CameraActor::updateSelf(float deltaSeconds)
{
	MovableActor::updateSelf(deltaSeconds);

	sf::View::setCenter(getWorldPosition());
}
