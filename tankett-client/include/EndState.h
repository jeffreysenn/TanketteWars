#pragma once
#include "StateBase.h"
#include "ScoreBoard.h"
#include <SFML/Graphics/Text.hpp>

namespace client
{

class EndState : public StateBase
{
public:
	EndState();

	virtual bool update(float deltaSeconds) override;
	virtual void draw() override;
	virtual bool handleEvent(const ::sf::Event& event) override;

	static void setScoreBoard(const ScoreBoard& scoreBoard);
	static ScoreBoard& getScoreBoard();

private:
	::sf::Text mTimerText;
};


}