#include "ConnectState.h"
#include "ClientNetworkManager.h"
#include "ClientContext.h"
#include "ClientStateStack.h"
#include "tankett_shared.h"

ConnectState::ConnectState()
	: MenuTemplate<Connect::Option>(Connect::OptionNames, static_cast<int>(Connect::Option::COUNT))
	, mNetworkManager(*ClientContext::getInstance().networkManager)
{
	mBackgroundShape.setFillColor(sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(sf::Vector2f(getRenderWindow().getSize()));

	mConnectText.setFont(*ClientContext::getInstance().fontManager->get(Font::MineCraft));
	mConnectText.setString("Connecting");
	mConnectText.setFillColor(sf::Color::Blue);
	mConnectText.setCharacterSize(50);
	helper::Graphics::centreOrigin(mConnectText);
	sf::Vector2u windowSize(getRenderWindow().getSize());
	mConnectText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);
}

bool ConnectState::update(float deltaSeconds)
{
	ClientNetworkManager::ConnectionState netState = mNetworkManager.getState();
	if(netState == ClientNetworkManager::ConnectionState::None)
		mNetworkManager.setState(ClientNetworkManager::ConnectionState::Discovering);

	if (netState == ClientNetworkManager::ConnectionState::Connected)
	{
		auto& receivedMessages = mNetworkManager.getReceivedMessages();
		for (auto& receivedMessage : receivedMessages)
		{
			network_message_type type = (network_message_type) receivedMessage->type_;
			switch (type)
			{
			case tankett::NETWORK_MESSAGE_SERVER_TO_CLIENT:
			{
				message_server_to_client* mS2C = static_cast<message_server_to_client*>(receivedMessage.get());

				if (mS2C->game_state == GAME_STATE::ROUND_RUNNING)
				{
					ClientStateStack& stack = *ClientContext::getInstance().stack;
					stack.clearStates();
					stack.pushState(StateID::Game);
				}
				
			} break;
			}
		}

		receivedMessages.clear();
	}

	return false;
}

void ConnectState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());

	getRenderWindow().draw(mBackgroundShape);
	getRenderWindow().draw(mConnectText);
	MenuTemplate<Connect::Option>::draw();
}

void ConnectState::handleConfirmInput()
{
	ClientStateStack& stack = *ClientContext::getInstance().stack;
	stack.popState();
}
