#pragma once
#include "MenuTemplate.h"
#include <SFML/Graphics/RectangleShape.hpp>

namespace Connect
{
	enum class Option
	{
		Back,
		COUNT,
	};

	static const char* OptionNames[static_cast<int>(Option::COUNT)] =
	{
		"Back",
	};
}


class ConnectState : public MenuTemplate<Connect::Option>
{
public:
	ConnectState();

	bool update(float deltaSeconds) override;

	void draw() override;

private:
	void handleConfirmInput() override;

	sf::RectangleShape mBackgroundShape;
	sf::Text mConnectText;
	class ClientNetworkManager& mNetworkManager;
};