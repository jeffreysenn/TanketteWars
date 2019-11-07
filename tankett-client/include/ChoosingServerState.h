#pragma once
#include "StateBase.h"
#include "alpha.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <map>
#include <vector>
namespace client
{

class ChoosingServerState : public StateBase
{
public:
	ChoosingServerState();

	bool update(float deltaSeconds) override;
	bool handleEvent(const ::sf::Event& event) override;

	void draw() override;

private:
	void incrementIndex(int dI);
private:
	::sf::RectangleShape mBackgroundShape;
	std::map<int, ::alpha::ip_address> mIndexIPMap;
	::sf::Text mBackText;
	::sf::Text mServerListText;
	std::vector<::sf::Text> mTexts;
	int mCurrentIndex;
};


}