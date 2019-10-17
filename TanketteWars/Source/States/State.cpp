#include "State.h"

#include "StateIdentifiers.h"
#include "StateStack.h"

State::State(StateStack & stack, const Context &context)
	: mStack(stack)
	, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(StateID stateID)
{
	mStack.pushState(stateID);
}

void State::requestStackPop()
{
	mStack.popState();
}

void State::requestStateClear()
{
	mStack.clearStates();
}