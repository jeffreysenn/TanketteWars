#include "WaitingState.h"
#include "ServerContext.h"
#include "tankett_shared.h"
#include "ServerNetworkManager.h"
#include "ServerStateStack.h"

WaitingState::WaitingState()
{
}

void WaitingState::processMessages()
{
	ServerNetworkManager& networkManager = *ServerContext::getInstance().networkManager;

	auto& clients = networkManager.getClients();
	if (clients.size() < 2)
	{
		for (auto& client : clients)
		{
			message_server_to_client* msgS2C = new message_server_to_client;
			msgS2C->game_state = GAME_STATE::WAITING_FOR_PLAYER;
			msgS2C->client_count = (uint8)clients.size();
			networkManager.pushMessage(client.first, msgS2C);
		}
	}
	else
	{
		for (auto& client : clients)
		{
			message_server_to_client* msgS2C = new message_server_to_client;
			msgS2C->game_state = GAME_STATE::ROUND_RUNNING;
			msgS2C->client_count = (uint8)clients.size();
			networkManager.pushMessage(client.first, msgS2C);
		}

		auto& stack = *ServerContext::getInstance().stack;
		stack.clearStates();
		stack.pushState(GAME_STATE::ROUND_RUNNING);
	}

	networkManager.clearAllClientsReceivedMessages();
}
