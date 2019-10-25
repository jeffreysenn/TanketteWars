#include "PauseState.h"
#include "ClientContext.h"
#include "ClientStateStack.h"

PauseState::PauseState()
	: MenuTemplate(PauseOptionNames, static_cast<int>(PauseOption::COUNT))
{
	mBackgroundShape.setFillColor(sf::Color(0, 0, 0, 80));
	mBackgroundShape.setSize(sf::Vector2f(getRenderWindow().getSize()));
}

void PauseState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());
	getRenderWindow().draw(mBackgroundShape);

	MenuTemplate<PauseOption>::draw();
}

void PauseState::handleConfirmInput()
{
	ClientStateStack& stack = *ClientContext::getInstance().stack;
	switch (getCurrentOption())
	{
	case PauseOption::Resume:
		stack.popState();
		break;
	case PauseOption::GoToMenu:
		stack.clearStates();
		stack.pushState(StateID::Menu);
		break;
	default:
		break;
	}
}
