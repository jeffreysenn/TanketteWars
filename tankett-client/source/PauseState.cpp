#include "PauseState.h"
#include "Context.h"
#include "ClientStateStack.h"
namespace client
{

PauseState::PauseState()
	: MenuTemplate(PauseOptionNames, static_cast<int>(PauseOption::COUNT))
{
	mBackgroundShape.setFillColor(::sf::Color(0, 0, 0, 80));
	mBackgroundShape.setSize(::sf::Vector2f(getRenderWindow().getSize()));
}

void PauseState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());
	getRenderWindow().draw(mBackgroundShape);

	MenuTemplate<PauseOption>::draw();
}

bool PauseState::update(float deltaSeconds)
{
	return true;
}

void PauseState::handleConfirmInput()
{
	ClientStateStack& stack = *Context::getInstance().stack;
	switch (getCurrentOption())
	{
	case PauseOption::Resume:
		stack.popState();
		break;
	case PauseOption::Disconnect:
		stack.clearStates();
		stack.pushState(StateID::Menu);
		break;
	default:
		break;
	}
}

}