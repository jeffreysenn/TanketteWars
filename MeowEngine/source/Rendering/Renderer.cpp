#include "Rendering/Renderer.h"

#include "SFML/Graphics/RenderTarget.hpp"
namespace mw
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::draw(::sf::RenderTarget& target, ::sf::RenderStates states) const
{
	for (auto const& renderInfoVector : mRenderBuffer)
	{
		for (auto const& renderInfo : renderInfoVector)
		{
			target.draw(*renderInfo.drawable, renderInfo.states);
		}
	}
}

void Renderer::pushRenderInfo(const RenderInfo renderInfo, Rendering::Layer layer)
{
	mRenderBuffer[layer].push_back(renderInfo);
}

void Renderer::clearRenderBuffer()
{
	for (auto& renderInfoVector : mRenderBuffer)
	{
		renderInfoVector.clear();
	}
}

}