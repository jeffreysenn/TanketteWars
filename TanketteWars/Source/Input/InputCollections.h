#pragma once
#include "Input.h"

namespace Input
{
	namespace Collections
	{
		static const InputCollection upInputs
		{
			{Type::Keyboard, sf::Keyboard::W},
			{Type::Keyboard, sf::Keyboard::Up}
		};

		static const InputCollection downInputs
		{
			{Type::Keyboard, sf::Keyboard::S},
			{Type::Keyboard, sf::Keyboard::Down}
		};

		static const InputCollection confirmInputs
		{
			{Type::Keyboard, sf::Keyboard::Enter},
			{Type::Keyboard, sf::Keyboard::Space}
		};
	}
}