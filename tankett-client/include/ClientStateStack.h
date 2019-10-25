#pragma once
#include "States/StateStack.h"
#include "ClientState.h"

namespace sf
{
	class Event;
	class RenderWindow;
	class Time;
}

class ClientStateStack : public StateStack<ClientState>
{
public:
	ClientStateStack();

	void update(float deltaSeconds);
	void draw();
	void handleEvent(const sf::Event& event);
};