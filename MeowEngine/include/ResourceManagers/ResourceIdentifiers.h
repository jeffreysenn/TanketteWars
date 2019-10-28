#pragma once

#include "ResourceManager.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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


	Dirt,
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
}
