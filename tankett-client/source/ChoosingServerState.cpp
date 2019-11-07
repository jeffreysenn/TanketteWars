#include "ChoosingServerState.h"
#include "NetworkManager.h"
#include "Context.h"
#include "Input/InputCollections.h"
#include "NetworkManager.h"
#include "ClientStateStack.h"
#include "StateIdentifiers.h"
#include <Helpers/Helper.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace client
{
ChoosingServerState::ChoosingServerState()
	: mCurrentIndex(0)
{
	const auto windowSize = getRenderWindow().getSize();

	mServerListText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mServerListText.setString("Choose Server and Press Enter");
	mServerListText.setCharacterSize(40);
	mw::helper::Graphics::centreOrigin(mServerListText);
	mServerListText.setPosition((float)windowSize.x / 2, 80.f);

	mBackText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mBackText.setString("Back");
	mBackText.setCharacterSize(40);
	mw::helper::Graphics::centreOrigin(mBackText);
	mBackText.setPosition((float)windowSize.x * 3 / 4, (float)windowSize.y - 80.f);
	mTexts.push_back(mBackText);

	mBackgroundShape.setFillColor(::sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(::sf::Vector2f(getRenderWindow().getSize()));
}

bool ChoosingServerState::update(float deltaSeconds)
{
	auto& networkManager = *Context::getInstance().networkManager;
	NetworkManager::ConnectionState netState = networkManager.getState();

	if (netState == NetworkManager::ConnectionState::None)
		networkManager.setState(NetworkManager::ConnectionState::Discovering);

	const auto& challenges = networkManager.getChallenges();
	for (const auto& challenge : challenges)
	{
		bool found = false;
		for (const auto& pair : mIndexIPMap)
		{
			if (pair.second == challenge.first)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			sf::Text text;
			text.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
			text.setString(challenge.first.as_string());
			mw::helper::Graphics::centreOrigin(text);
			int index = (int)mTexts.size() - 1;
			const auto windowSize = getRenderWindow().getSize();
			text.setPosition((float)windowSize.x / 2.f, 160.f + 40.f * index);
			mTexts.insert(--mTexts.end(), text);
			mIndexIPMap[index] = challenge.first;
		}

	}

	for (auto& text : mTexts)
	{
		text.setFillColor(sf::Color::Blue);
	}
	mTexts[mCurrentIndex].setFillColor(sf::Color::Green);

	return false;
}

bool ChoosingServerState::handleEvent(const::sf::Event& event)
{
	if (event.type != ::sf::Event::KeyPressed)
		return false;

	if (Input::eventInputCollectionPressed(event, Input::Collections::confirmInputs))
	{
		ClientStateStack& stack = *Context::getInstance().stack;
		auto& networkManager = *Context::getInstance().networkManager;
		if (mCurrentIndex == mTexts.size() - 1)
		{
			stack.popState();
			networkManager.resetNetworkManager();
		}
		else
		{
			networkManager.acceptChallenge(mIndexIPMap[mCurrentIndex]);
			stack.popState();
			stack.pushState(StateID::Connect);
		}
	}

	if (Input::eventInputCollectionPressed(event, Input::Collections::upInputs))
		incrementIndex(-1);

	if (Input::eventInputCollectionPressed(event, Input::Collections::downInputs))
		incrementIndex(1);
	return false;
}

void ChoosingServerState::draw()
{
	getRenderWindow().draw(mBackgroundShape);
	getRenderWindow().draw(mServerListText);
	for (const auto& text : mTexts)
	{
		getRenderWindow().draw(text);
	}
}
void ChoosingServerState::incrementIndex(int dI)
{
	int nextIndex = mCurrentIndex + dI;
	if (nextIndex < 0)
	{
		mCurrentIndex = 0;
	}
	else if (nextIndex > mTexts.size() - 1)
	{
		mCurrentIndex = (int) mTexts.size() - 1;
	}
	else
	{
		mCurrentIndex = nextIndex;
	}
}
}