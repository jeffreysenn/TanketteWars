#include "ResourceManagers/Map.h"
#include "Actors/Obstacle.h"

#include <fstream>
#include <sstream>

namespace mw
{
	namespace rs
	{
		Map::Map()
			: mRow(0)
			, mCol(0)
		{
		}

		bool Map::loadFromFile(const ::std::string& filename)
		{
			::std::ifstream file(filename);
			if (!file.is_open())
				return false;

			::std::string line;
			while (getline(file, line))
			{
				::std::istringstream iss(line);
				bool isWall = false;
				while (iss >> isWall)
				{
					if (mRow == 0)
						++mCol;

					mMatrix.push_back(isWall);
				}

				++mRow;
			}

			file.close();
			return true;
		}

		::std::unique_ptr<Actor> Map::buildMap(float width, float height, const ::sf::Texture& texture)
		{
			::std::unique_ptr<Actor> map = ::std::make_unique<Actor>();
			for (uint32_t i = 0; i < mRow; ++i)
			{
				for (uint32_t j = 0; j < mCol; ++j)
				{
					if (mMatrix[static_cast<uint64_t>(mCol)* i + j])
					{
						::std::unique_ptr<Obstacle> obstacle = ::std::make_unique<Obstacle>(width, height, texture);
						obstacle->setPosition(width * j, height * i);
						map->attachChild(::std::move(obstacle));
					}
				}
			}
			return map;
		}
	}



}