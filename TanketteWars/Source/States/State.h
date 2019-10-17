#pragma once
#include "Context.h"

#include <memory>

class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	explicit State(StateStack &stack, const Context &context);
	virtual ~State();

	virtual bool update(float deltaSeconds) { return true; }
	virtual bool handleEvent(const sf::Event &event) { return true; }
	virtual void draw() {}

protected:
	void requestStackPush(enum class StateID stateID);
	void requestStackPop();
	void requestStateClear();

	Context getContext() const { return mContext; }
	sf::RenderWindow& getRenderWindow() const { return *mContext.window; }
private:
	StateStack &mStack;
	Context mContext;
};

