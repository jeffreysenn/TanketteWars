#pragma once
#include "ServerState.h"
#include "PlayerController.h"
#include <SFML/System/Clock.hpp>
#include <map>

namespace server
{

class EndState : public ServerState
{
public:
	void processMessages() override;
	void packMessages() override;

private:
	::sf::Clock mStateClock;
};

}