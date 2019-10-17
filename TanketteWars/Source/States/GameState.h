#pragma once
#include "State.h"
#include "../Worlds/World.h"
#include "../Rendering/Renderer.h"
#include "../Input/Input.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State
{
public:
	GameState(StateStack &stateStack, const Context &context);
	~GameState();

	virtual bool update(float deltaSeconds) override;
	virtual void draw() override;
	virtual bool handleEvent(const sf::Event& event) override;

private:
	World mWorld;
	PlayerController& mPlayerController;
	Renderer mRenderer;
	Input::InputCollection mPauseInputs{
		{ Input::Type::Keyboard, sf::Keyboard::Escape },
		{ Input::Type::Keyboard, sf::Keyboard::BackSpace }};

};