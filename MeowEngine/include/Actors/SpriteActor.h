#pragma once
#include "MovableActor.h"

#include "Rendering/RenderInfo.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace mw
{
	class SpriteActor : public MovableActor
	{
	public:
		SpriteActor();
		explicit SpriteActor(const ::sf::Texture& texture, Rendering::Layer layer = Rendering::Default);

		void centreOrigin();

		void setLayer(Rendering::Layer layer) { mLayer = layer; }
		void setSpriteRect(::sf::IntRect rect);
		Rendering::Layer getLayer() const { return mLayer; }

		::sf::Sprite* getSprite() { return &mSprite; }
		const ::sf::Sprite* getSprite() const { return &mSprite; }

	protected:
		virtual void updateSelf(float deltaSeconds) override;

		virtual void reportRenderInfoSelf(class Renderer& renderer, ::sf::RenderStates states) const override;

	protected:
		::sf::Sprite mSprite;

		Rendering::Layer mLayer;
	};


}