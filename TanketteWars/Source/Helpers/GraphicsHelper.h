#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

namespace GraphicsHelper
{
	template<typename T>
	void centreOrigin(T &object)
	{
		sf::FloatRect localBounds(object.getLocalBounds());
		object.setOrigin(localBounds.left + localBounds.width / 2,
						 localBounds.top + localBounds.height / 2);
	}
}