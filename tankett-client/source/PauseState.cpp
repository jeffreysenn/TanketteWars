#include "PauseState.h"
#include "Context.h"
#include "ClientStateStack.h"
#include "NetworkManager.h"
namespace client
{

PauseState::PauseState()
	: MenuTemplate(PauseOptionNames, static_cast<int>(PauseOption::COUNT))
	, mDisconnecting(false)
{
	mBackgroundShape.setFillColor(::sf::Color(0, 0, 0, 80));
	mBackgroundShape.setSize(::sf::Vector2f(getRenderWindow().getSize()));
	mDisconnectingText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mDisconnectingText.setString("Disconnecting...");
	mDisconnectingText.setFillColor(::sf::Color::White);
	mDisconnectingText.setCharacterSize(50);
	helper::Graphics::centreOrigin(mDisconnectingText);
	::sf::Vector2u windowSize(getRenderWindow().getSize());
	mDisconnectingText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);
}

void PauseState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());
	getRenderWindow().draw(mBackgroundShape);
	if (mDisconnecting)
	{
		getRenderWindow().draw(mDisconnectingText);
	}

	MenuTemplate<PauseOption>::draw();
}

bool PauseState::update(float deltaSeconds)
{
	if (mDisconnecting)
	{
		if (mDisconnectClock.getElapsedTime().asSeconds() > 4.f)
		{
			ClientStateStack& stack = *Context::getInstance().stack;
			stack.clearStates();
			stack.pushState(StateID::Menu);
			mDisconnecting = false;
		}
		return false;
	}
	return true;
}

void PauseState::handleConfirmInput()
{
	ClientStateStack& stack = *Context::getInstance().stack;
	if (!mDisconnecting)
	{
		switch (getCurrentOption())
		{
		case PauseOption::Resume:
			stack.popState();
			break;
		case PauseOption::Disconnect:
			Context::getInstance().networkManager->resetNetworkManager();
			mDisconnecting = true;
			mDisconnectClock.restart();
			break;
		default:
			break;
		}
	}
}

}