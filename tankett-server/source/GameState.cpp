#include "GameState.h"
#include "Context.h"
#include "tankett_shared.h"
#include "NetworkManager.h"
#include "ServerStateStack.h"
#include "Unit.h"
#include "Tank.h"
#include "Bullet.h"
#include "tankett_debug.h"
#include "EliminationHeatMap.h"
#include "Unit.h"
#include "EndState.h"

#include <vector>

namespace server
{

constexpr float ROUND_LENGTH = 90.f;

GameState::GameState()
	: mNetworkManager(*Context::getInstance().networkManager)
	, mRoundClock()
{
	mNetworkManager.resetClientsLatestReceivedInputSequence();
}

void GameState::processMessages()
{
	checkJoin();
	checkQuit();

	checkRespawn();

	if (!mNetworkManager.allClientReceivedMessagesEmpty())
	{
		applyInput();
	}
	checkTime();
}

void GameState::checkRespawn()
{
	auto& clients = mNetworkManager.getClients();
	for (auto& client : clients)
	{
		uint8_t id = client.second.id;

		auto& controller = mControllers[id];

		if (!controller.getPossessedTank())
		{
			if (mRespawnMap[id] == false)
			{
				auto killPos = controller.getKillPos();
				int x = (int)::tankett::unit::pix2unit(killPos.x);
				int y = (int)::tankett::unit::pix2unit(killPos.y);
				EliminationHeatMap* heatMap = Context::getInstance().heatMapManager->get(EliminationHeatMap::ID::DefaultMap);
				heatMap->addKillAtTile(x, y);

				mRespawnMap[id] = true;
				mRespawnClocks[id].restart();
			}
			if (mRespawnClocks[id].getElapsedTime().asMilliseconds() >= 3000)
			{
				mRespawnMap[id] = false;
				controller.spawnTank_server(mWorld.getTankManager());
			}
		}
	}
}

void GameState::checkJoin()
{
	auto& clients = mNetworkManager.getClients();

	for (auto& client : clients)
	{
		uint8_t id = client.second.id;
		// insert controllers
		if (mControllers.find(id) == mControllers.end())
		{
			::mw::CommandCategory commandCategory =
				(::mw::CommandCategory)((uint32_t)::mw::CommandCategory::Tank0 << client.second.id);
			::tankett::PlayerController controller(&mWorld.getSceneGraph(), client.second.id);
			mControllers.insert(::std::make_pair(id, controller));
			mControllers[id].spawnTank_server(mWorld.getTankManager());
		}
	}
}

void GameState::checkQuit()
{
	auto& clients = mNetworkManager.getClients();
	for (auto it = mControllers.begin(); it != mControllers.end();)
	{
		bool found = false;
		for (const auto& client : clients)
		{
			if (client.second.id == it->second.getID())
				found = true;
		}
		if (!found)
		{
			it = mControllers.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void GameState::applyInput()
{
	auto& clients = mNetworkManager.getClients();

	for (auto& client : clients)
	{
		const auto& receivedMessages = client.second.receivedMessages;
		uint32_t inputCount = 0;
		for (const auto& msg : receivedMessages)
		{
			network_message_type type = (network_message_type)msg->type_;
			if (type == ::tankett::NETWORK_MESSAGE_CLIENT_TO_SERVER)
				++inputCount;
		}

		for (const auto& msg : receivedMessages)
		{
			network_message_type type = (network_message_type)msg->type_;
			switch (type)
			{
			case ::tankett::NETWORK_MESSAGE_CLIENT_TO_SERVER:
			{
				message_client_to_server* msgC2S = (message_client_to_server*)msg;
				if (!msgC2S) break;
				auto& controller = mControllers[client.second.id];
				bool up = msgC2S->get_input(message_client_to_server::UP);
				bool down = msgC2S->get_input(message_client_to_server::DOWN);
				bool left = msgC2S->get_input(message_client_to_server::LEFT);
				bool right = msgC2S->get_input(message_client_to_server::RIGHT);
				bool fire = msgC2S->get_input(message_client_to_server::SHOOT);
				float aimAngle = msgC2S->turret_angle;
				float deltaSeconds = 1.f / PROTOCOL_SEND_PER_SEC / (inputCount * 1.f);
				controller.updateTank(up, down, left, right, fire, aimAngle, deltaSeconds, msgC2S->input_number);
			} break;
			default:
				break;
			}
		}
	}

	mNetworkManager.clearAllClientsReceivedMessages();

}

void GameState::packMessages()
{
	if (mControllers.size() == 0) return;

	auto& clients = mNetworkManager.getClients();
	PlayerState stateArr[4];
	int stateIndex = 0;
	for (auto& client : clients)
	{
		uint8_t id = client.second.id;
		auto& state = stateArr[stateIndex++];
		state.client_id = id;
		state.ping = client.second.ping;
		state.eliminations = mControllers[id].getScore();
		const auto tank = mControllers[id].getPossessedTank();
		if (tank)
		{
			state.alive = true;
			state.angle = tank->getTurretAngle();
			const auto& pos = tank->getPosition();
			state.position = alpha::vector2(::tankett::unit::pix2unit(pos.x), ::tankett::unit::pix2unit(pos.y));
		}
		else
		{
			state.alive = false;
		}

		const auto& bullets = mControllers[id].getBullets();
		state.bullet_count = (uint8_t)bullets.size();
		for (int i = 0; i < bullets.size(); ++i)
		{
			bullet_data bulletData;
			bulletData.id = bullets[i]->getID();
			const auto bulletPos = bullets[i]->getPosition();
			bulletData.position = ::alpha::vector2(::tankett::unit::pix2unit(bulletPos.x), ::tankett::unit::pix2unit(bulletPos.y));
			state.bullets[i] = bulletData;
		}
	}

	for (auto& client : clients)
	{
		message_server_to_client* message = new message_server_to_client;
		message->receiver_id = client.second.id;
		message->sendTime = ::alpha::time::now().as_milliseconds();
		message->round_time = ROUND_LENGTH - mRoundClock.getElapsedTime().asSeconds();
		message->input_number = client.second.latestReceivedInputSequence;

		message->game_state = GAME_STATE::ROUND_RUNNING;
		message->client_count = (uint8_t)clients.size();
		memcpy(message->client_data, stateArr, sizeof(stateArr));

		mNetworkManager.pushMessage(client.first, message);
	}
}
void GameState::checkTime()
{
	if (ROUND_LENGTH - mRoundClock.getElapsedTime().asSeconds() < 0)
	{
		auto& stack = Context::getInstance().stack;
		stack->clearStates();
		stack->pushState(GAME_STATE::ROUND_END);
	}
}
}