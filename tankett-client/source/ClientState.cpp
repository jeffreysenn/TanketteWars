#include "ClientState.h"
#include "ClientContext.h"

ClientState::ClientState()
{
}

ClientState::~ClientState()
{
}

ClientContext& ClientState::getContext() const
{
	return ClientContext::getInstance();
}

sf::RenderWindow& ClientState::getRenderWindow() const
{
	return *ClientContext::getInstance().window;
}
