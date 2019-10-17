#include "StateStack.h"

#include "State.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

StateStack::StateStack(const Context &context)
	: mContext(context)
{
}

void StateStack::update(float deltaSeconds)
{
	for (auto rit = mStack.rbegin(); rit != mStack.rend(); ++rit)
	{
		if (!(*rit)->update(deltaSeconds))
			break;
	}
	applyPendingChanges();
}

void StateStack::draw()
{
	for (const auto &state : mStack)
	{
		state->draw();
	}
}

void StateStack::handleEvent(const sf::Event & event)
{
	for (auto rit = mStack.rbegin(); rit != mStack.rend(); ++rit)
	{	
		if (!(*rit)->handleEvent(event)) 
			break;
	}
	applyPendingChanges();
}

void StateStack::pushState(StateID stateID)
{
	mPendingList.push_back(PendingChange(Action::Push, stateID));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::createState(StateID stateID)
{
	assert(mFactories[stateID] != nullptr);
	return mFactories[stateID]();
}

void StateStack::applyPendingChanges()
{
	for (auto const &change : mPendingList)
	{
		switch (change.action)
		{
		case Action::Push:
			mStack.push_back(createState(change.stateID));
			break;

		case Action::Pop:
			mStack.pop_back();
			break;

		case Action::Clear:
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}
