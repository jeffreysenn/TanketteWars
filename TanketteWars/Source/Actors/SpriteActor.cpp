#include "SpriteActor.h"

#include "../Rendering/Renderer.h"

#include <iostream>

SpriteActor::SpriteActor()
	: mLayer(Rendering::Layer::Default)
{
}

SpriteActor::SpriteActor(const sf::Texture& texture, Rendering::Layer layer)
	: mSprite(texture)
	, mLayer(layer)
{
	centreOrigin();
}

void SpriteActor::centreOrigin()
{
	sf::FloatRect spriteBounds(mSprite.getLocalBounds());
	mSprite.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
}

void SpriteActor::setSpriteRect(sf::IntRect rect)
{
	mSprite.setTextureRect(rect); 
	centreOrigin();
}

void SpriteActor::reportRenderInfoSelf(Renderer & renderer, sf::RenderStates states) const
{
	RenderInfo renderInfo(mSprite, states); 
	renderer.pushRenderInfo(renderInfo, mLayer);
}

void SpriteActor::updateSelf(float deltaSeconds)
{
	MovableActor::updateSelf(deltaSeconds);
}