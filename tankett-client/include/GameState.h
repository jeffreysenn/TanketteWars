#pragma once
#include "ClientState.h"
#include "World.h"
#include "Rendering/Renderer.h"
#include "Input/Input.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public ClientState
{
public:
	GameState();
	~GameState();

	virtual bool update(float deltaSeconds) override;
	virtual void draw() override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	World mWorld;
	class PlayerController& mPlayerController;
	Renderer mRenderer;
	Input::InputCollection mPauseInputs{
		{ Input::Type::Keyboard, sf::Keyboard::Escape },
		{ Input::Type::Keyboard, sf::Keyboard::BackSpace }};
	uint32_t mFrameNum;
};