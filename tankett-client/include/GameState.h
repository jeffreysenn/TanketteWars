#pragma once
#include "StateBase.h"
#include "World.h"
#include "Rendering/Renderer.h"
#include "Input/Input.h"
#include "PlayerController.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

using namespace mw;
using namespace tankett;

namespace client
{
class GameState : public StateBase
{
public:
	GameState();
	~GameState();

	virtual bool update(float deltaSeconds) override;
	virtual void draw() override;
	virtual bool handleEvent(const ::sf::Event& event) override;

private:
	World mWorld;
	::std::vector<::std::unique_ptr<PlayerController>> mPlayerControllers;
	Renderer mRenderer;
	Input::InputCollection mPauseInputs{
		{ Input::Type::Keyboard, ::sf::Keyboard::Escape },
		{ Input::Type::Keyboard, ::sf::Keyboard::BackSpace } };
	uint32_t mFrameNum;
};
}