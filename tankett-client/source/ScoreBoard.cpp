#include "ScoreBoard.h"
#include "Context.h"
#include <SFML/Graphics/RenderTarget.hpp>

namespace client
{
ScoreBoard::ScoreBoard()
{
}

constexpr float BLOCK_DISTANCE = 40.f;
constexpr float LINE_DISTANCE = 10.f;
void ScoreBoard::setScoreBoard(uint8_t id, uint8_t score, uint32_t ping)
{
	auto& texts = mTextMap[id];
	texts.id.setString("player: " + ::std::to_string(id));
	texts.score.setString("score: " + ::std::to_string(score));
	texts.ping.setString("ping: " + ::std::to_string(ping));

	texts.id.setPosition(0, id * BLOCK_DISTANCE);
	texts.score.setPosition(0, id * BLOCK_DISTANCE + LINE_DISTANCE);
	texts.ping.setPosition(0, id * BLOCK_DISTANCE + LINE_DISTANCE * 2);
}

void ScoreBoard::draw(::sf::RenderTarget& target, ::sf::RenderStates states) const
{
	
	for (const auto& pair : mTextMap)
	{
		const auto& texts = pair.second;
		target.draw(texts.id, ::sf::RenderStates(getTransform() * texts.id.getTransform()));
		target.draw(texts.score, ::sf::RenderStates(getTransform() * texts.score.getTransform()));
		target.draw(texts.ping, ::sf::RenderStates(getTransform() * texts.ping.getTransform()));
	}
}
ScoreBoard::TextBlock::TextBlock()
{
	auto& font = *Context::getInstance().fontManager->get(Font::Sansation);
	id.setFont(font);
	id.setString("player: 0");
	id.setCharacterSize(14);
	score.setFont(font);
	score.setString("score: 0");
	score.setCharacterSize(14);
	ping.setFont(font);
	ping.setString("ping: 0");
	ping.setCharacterSize(14);
}
}