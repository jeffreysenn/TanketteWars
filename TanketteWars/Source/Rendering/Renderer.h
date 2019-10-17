#pragma once
#include <SFML/Graphics/Drawable.hpp>

#include <array>
#include <vector>

#include "RenderInfo.h"

class Renderer : public sf::Drawable
{
public:
	Renderer();
	~Renderer();

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;

	void pushRenderInfo(const RenderInfo renderInfo, Rendering::Layer layer);

	void clearRenderBuffer();

private:
	std::array<std::vector<RenderInfo>, Rendering::LayerCount> mRenderBuffer;
};

