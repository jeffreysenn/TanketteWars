#include "EndState.h"
#include "Context.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"


namespace server
{
constexpr float STATE_LENGTH = 15.f;
void EndState::processMessages()
{
	if (mStateClock.getElapsedTime().asSeconds() > STATE_LENGTH)
	{
		auto& stack = Context::getInstance().stack;
		stack->clearStates();
		stack->pushState(GAME_STATE::WAITING_FOR_PLAYER);
	}
}

void EndState::packMessages()
{
	auto& networkManager = *Context::getInstance().networkManager;
	auto& clients = networkManager.getClients();
	for (auto& client : clients)
	{
		message_server_to_client* message = new message_server_to_client;
		message->receiver_id = client.second.id;
		message->sendTime = ::alpha::time::now().as_milliseconds();
		message->round_time = STATE_LENGTH - mStateClock.getElapsedTime().asSeconds();

		message->game_state = GAME_STATE::ROUND_END;
		message->client_count = (uint8_t)clients.size();

		networkManager.pushMessage(client.first, message);
	}
}

}