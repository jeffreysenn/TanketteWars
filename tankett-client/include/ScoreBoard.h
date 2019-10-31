#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <map>

namespace client
{

class ScoreBoard : public ::sf::Drawable, public ::sf::Transformable
{

public:
	ScoreBoard();

	void setScoreBoard(uint8_t id, uint8_t score, uint32_t ping);

	void draw(::sf::RenderTarget& target, ::sf::RenderStates states = ::sf::RenderStates::Default) const override;

private:
	struct TextBlock
	{
		TextBlock();
		::sf::Text id;
		::sf::Text score;
		::sf::Text ping;
	};

private:
	::std::map<uint8_t, TextBlock> mTextMap;
};
}