#include "GameState.h"
#include "Context.h"
#include "tankett_shared.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"
namespace server
{

GameState::GameState()
{

}

void GameState::processMessages()
{
	NetworkManager& networkManager = *Context::getInstance().networkManager;
	auto& clients = networkManager.getClients();
	for (auto& client : clients)
	{
		if (client.second.sendMessageQueue.size() == 0)
		{
			message_server_to_client* msgS2C = new message_server_to_client;
			msgS2C->game_state = GAME_STATE::ROUND_RUNNING;
			msgS2C->client_count = (uint8)clients.size();
			networkManager.pushMessage(client.first, msgS2C);
		}
	}
}

}