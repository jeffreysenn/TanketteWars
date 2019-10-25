#pragma once
#include "CollisionIdentifiers.h"
#include <SFML/Graphics/Rect.hpp>

namespace mw
{

	struct Collider
	{
		explicit Collider(Collision::ObjectType type = Collision::ObjectType::None,
						  Collision::ObjectResponse response = Collision::ObjectResponse(Collision::ObjectResponsePreset::NoCollision),
						  ::sf::FloatRect rect = ::sf::FloatRect())
			: type(type)
			, response(response)
			, rect(rect)
		{
		}

		Collision::ObjectType type;
		Collision::ObjectResponse response;
		::sf::FloatRect rect;
	};
}