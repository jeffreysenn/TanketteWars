#include "MenuState.h"
#include "Context.h"
#include "ClientStateStack.h"
namespace client
{
MenuState::MenuState()
	: MenuTemplate(Menu::OptionNames, static_cast<int>(Menu::Option::COUNT))
{
	mBackgroundShape.setFillColor(::sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(::sf::Vector2f(getRenderWindow().getSize()));
}

void MenuState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());
	getRenderWindow().draw(mBackgroundShape);
	drawOptions();
}

void MenuState::handleConfirmInput()
{
	ClientStateStack& stack = *Context::getInstance().stack;

	switch (getCurrentOption())
	{
	case Menu::Option::Connect:
		stack.pushState(StateID::ChoosingServer);
		break;
	case Menu::Option::Settings:
		stack.pushState(StateID::Setting);
		break;
	case Menu::Option::Credits:
		stack.pushState(StateID::Credit);
		break;
	case Menu::Option::Exit:
		stack.clearStates();
		break;
	default:
		break;
	}
}
}