#pragma once
#include "States/StateStack.h"
#include "ClientState.h"

using namespace mw;

namespace sf
{
	class Event;
}

class ClientStateStack : public StateStack<ClientState>
{
public:
	ClientStateStack();

	void update(float deltaSeconds);
	void draw();
	void handleEvent(const sf::Event& event);
};