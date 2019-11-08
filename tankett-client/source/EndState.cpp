#include "EndState.h"
#include "Context.h"
#include "NetworkManager.h"
#include "ClientStateStack.h"
#include "StateIdentifiers.h"
#include "Helpers/Helper.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace client
{

EndState::EndState()
{
	mTimerText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mTimerText.setString("Starting in 15 seconds");
	mTimerText.setCharacterSize(60);
	mTimerText.setPosition(Context::getInstance().window->getSize().x / 2.f, 50.f);
	::mw::helper::Graphics::centreOrigin(mTimerText);

	getScoreBoard().setPosition(Context::getInstance().window->getSize().x / 2.f, 200.f);
}

bool EndState::update(float deltaSeconds)
{
	auto& networkManager = *Context::getInstance().networkManager;
	auto& receivedMessages = networkManager.getReceivedMessages();

	for (auto& message : receivedMessages)
	{
		::tankett::network_message_type type = (::tankett::network_message_type)message->type_;
		switch (type)
		{
		case tankett::NETWORK_MESSAGE_SERVER_TO_CLIENT:
		{
			::tankett::message_server_to_client* msgS2C = (::tankett::message_server_to_client*)message.get();

			auto state = (::tankett::GAME_STATE)msgS2C->game_state;
			auto& stack = *Context::getInstance().stack;
			switch (state)
			{
			case tankett::GAME_STATE::ROUND_RUNNING:
				stack.clearStates();
				stack.pushState(StateID::Game);
				break;
			case tankett::GAME_STATE::WAITING_FOR_PLAYER:
				stack.clearStates();
				stack.pushState(StateID::Connect);
				break;
			case tankett::GAME_STATE::ROUND_END:
				break;
			default:
				break;
			}

			int remainingTime = (int)msgS2C->round_time;
			mTimerText.setString("Starting in " + ::std::to_string(remainingTime) + " seconds");
		}
		break;
		default:
			break;
		}
	}
	networkManager.clearReceivedMessages();
	return false;
}
void EndState::draw()
{
	auto& window = Context::getInstance().window;
	window->setView(window->getDefaultView());
	window->draw(mTimerText);
	window->draw(getScoreBoard());
}

bool EndState::handleEvent(const::sf::Event& event)
{
	if (Input::eventInputCollectionPressed(event, mPauseInputs))
		Context::getInstance().stack->pushState(StateID::Pause);

	return false;
}

static ScoreBoard mScoreBoard;
void EndState::setScoreBoard(const ScoreBoard& scoreBoard)
{
	mScoreBoard = scoreBoard;
}

ScoreBoard& EndState::getScoreBoard()
{
	return mScoreBoard;
}
}