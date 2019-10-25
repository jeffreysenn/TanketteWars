#include "Input/Input.h"
#include <cassert>
namespace mw
{

	bool inputPressed(const Input::Input& input)
	{
		switch (input.type)
		{
			// TODO: Implement Joystick index to support multiple joysticks
		case Input::Type::Joystick:
			return ::sf::Joystick::isButtonPressed(0, input.joystickButtonID);
			break;
		case Input::Type::Keyboard:
			return ::sf::Keyboard::isKeyPressed(input.keyboardKey);
			break;
		case Input::Type::Mouse:
			return ::sf::Mouse::isButtonPressed(input.mouseButton);
			break;
		default:
			break;
		}

		return false;
	}

	bool Input::inputCollectionPressed(const InputCollection& inputCollection)
	{
		for (auto const& input : inputCollection)
		{
			if (inputPressed(input))
				return true;
		}
		return false;
	}

	bool eventInputPressed(const ::sf::Event& event, const Input::Input& input)
	{
		bool valid = true;
		switch (event.type)
		{
		case ::sf::Event::KeyPressed:
			valid = (input.type == Input::Type::Keyboard);
			break;
		case ::sf::Event::MouseButtonPressed:
			valid = (input.type == Input::Type::Mouse);
			break;
		}
		if (!valid)
			return false;

		switch (input.type)
		{
		case Input::Type::Joystick:
			return event.joystickButton.button == input.joystickButtonID;
		case Input::Type::Keyboard:
			return event.key.code == input.keyboardKey;
		case Input::Type::Mouse:
			return event.mouseButton.button == input.mouseButton;
		}

		return false;
	}

	bool Input::eventInputCollectionPressed(const ::sf::Event& event, const InputCollection& inputCollection)
	{

		if (event.type != ::sf::Event::KeyPressed && event.type != ::sf::Event::MouseButtonPressed)
			return false;

		for (auto const& input : inputCollection)
		{
			if (eventInputPressed(event, input))
				return true;
		}
		return false;
	}

}