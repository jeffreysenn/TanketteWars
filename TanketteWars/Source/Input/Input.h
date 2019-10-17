#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>


namespace Input
{
	enum class Type
	{
		Joystick,
		Keyboard,
		Mouse,
	};

	struct Input
	{
		Type type;

		union
		{
			uint32_t joystickButtonID;
			sf::Joystick::Axis joystickAxis;

			sf::Keyboard::Key keyboardKey;

			sf::Mouse::Button mouseButton;
		};
	};

	typedef std::vector<Input> InputCollection;
	bool inputCollectionPressed(const InputCollection &inputCollection);
	bool eventInputCollectionPressed(const sf::Event &event, const InputCollection &inputCollection);
}

