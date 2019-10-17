#pragma once
#include <SFML/System/NonCopyable.hpp>

#include "State.h"
#include "StateIdentifiers.h"

#include <vector>
#include <memory>
#include <functional>
#include <map>

namespace sf
{
	class Event;
	class RenderWindow;
	class Time;
}

class StateStack : sf::NonCopyable
{
public:
	enum class Action
	{
		Push,
		Pop,
		Clear
	};

public:
	explicit StateStack(const Context &context);

	template <typename T>
	void registerState(StateID stateID);

	void update(float deltaSeconds);
	void draw();
	void handleEvent(const sf::Event& event);

	void pushState(StateID stateID);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::Ptr createState(StateID stateID);
	void applyPendingChanges();

private:
	struct PendingChange
	{
		explicit PendingChange(const Action &action, 
							   const StateID &stateID = StateID::None)
			: action(action)
			, stateID(stateID)
		{
		}

		Action action;
		StateID stateID;
	};

private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;

	Context mContext;
	std::map<StateID,
		std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(StateID stateID)
{
	mFactories[stateID] = [this]()
	{
		return State::Ptr(std::make_unique<T>(*this, mContext));
	};
}