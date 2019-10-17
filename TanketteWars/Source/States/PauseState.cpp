#include "PauseState.h"

PauseState::PauseState(StateStack & stateStack, const Context & context)
	: MenuTemplate(stateStack, context, PauseOptionNames, static_cast<int>(PauseOption::COUNT))
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
	switch (getCurrentOption())
	{
	case PauseOption::Resume:
		requestStackPop();
		break;
	case PauseOption::GoToMenu:
		requestStateClear();
		requestStackPush(StateID::Menu);
		break;
	default:
		break;
	}
}
