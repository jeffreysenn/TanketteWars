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
	Back,

	TankBlackHull,
	TankBlackBarrel,
	TankBlackBullet,

	TankRedHull,
	TankRedBarrel,
	TankRedBullet,

	TankGreenHull,
	TankGreenBarrel,
	TankGreenBullet,

	TankBlueHull,
	TankBlueBarrel,
	TankBlueBullet,

	Grass,
	Dirt,
	NONEXIST
};
}
typedef ResourceManager<::sf::Texture, Texture::ID> TextureManager;


namespace Font
{
enum ID
{
	Sansation
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
