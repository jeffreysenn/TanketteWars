#pragma once

#include "ResourceManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Map.h"

namespace mw
{
	namespace Texture
	{
		enum ID
		{
			Avatar,
			Back,
			Middle,
			TankBlackHull,
			TankBlackBarrel,
			TankBlackBullet,


			Oil,
			NONEXIST
		};
	}
	typedef ResourceManager<::sf::Texture, Texture::ID> TextureManager;


	namespace Font
	{
		enum ID
		{
			MineCraft
		};
	}
	typedef ResourceManager<::sf::Font, Font::ID> FontManager;


	namespace Sound
	{
		enum ID
		{

		};
	}
	typedef ResourceManager<::sf::SoundBuffer, Sound::ID> SoundManager;

	namespace Map
	{
		enum ID
		{
			DefaultMap,
		};
	}
	typedef ResourceManager<rs::Map, Map::ID> MapManager;
}