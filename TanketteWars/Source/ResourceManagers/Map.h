#pragma once
#include "../Actors/Actor.h"

#include <string>
#include <vector>

namespace rs
{
	class Map
	{
	public:
		Map();

		bool loadFromFile(const std::string& filename);

		std::unique_ptr<Actor> buildMap(float width, float height, const sf::Texture& texture);

	private:
		size_t mRow;
		size_t mCol;
		std::vector<bool> mMatrix;
	};
}