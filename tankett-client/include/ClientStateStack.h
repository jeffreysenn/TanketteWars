#pragma once
#include "States/StateStack.h"
#include "StateBase.h"

using namespace mw;

namespace sf
{
class Event;
}

namespace client
{
class ClientStateStack : public StateStack<StateBase>
{
public:
	ClientStateStack();

	void update(float deltaSeconds);
	void draw();
	void handleEvent(const ::sf::Event& event);
};
}