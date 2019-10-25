#include "ClientStateStack.h"
#include "ClientState.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

ClientStateStack::ClientStateStack()
{
}

void ClientStateStack::update(float deltaSeconds)
{
	for (auto rit = mStack.rbegin(); rit != mStack.rend(); ++rit)
	{
		if (!(*rit)->update(deltaSeconds))
			break;
	}

	applyPendingChanges();
}

void ClientStateStack::draw()
{
	for (const auto &state : mStack)
	{
		state->draw();
	}
}

void ClientStateStack::handleEvent(const sf::Event & event)
{
	for (auto rit = mStack.rbegin(); rit != mStack.rend(); ++rit)
	{	
		if (!(*rit)->handleEvent(event)) 
			break;
	}
	applyPendingChanges();
}