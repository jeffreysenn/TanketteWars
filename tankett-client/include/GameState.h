#pragma once
#include "StateBase.h"
#include "World.h"
#include "Rendering/Renderer.h"
#include "Input/Input.h"
#include "PlayerController.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace tankett
{
struct message_server_to_client;
}

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
	void processReceivedMessages();
	void checkNewRemote(::tankett::message_server_to_client* msgS2C);
	void updateRemoteState(::tankett::message_server_to_client* msgS2C);
	void packInput();

	void pushInputMessage(::tankett::PlayerController::TankInput& inputValue, uint32_t inputNum);

private:
	World mWorld;
	::std::vector<::std::unique_ptr<::tankett::PlayerController>> mPlayerControllers;
	::tankett::PlayerController* mLocalController{};
	::mw::Renderer mRenderer;
	::mw::Input::InputCollection mPauseInputs{
		{ Input::Type::Keyboard, ::sf::Keyboard::Escape },
		{ Input::Type::Keyboard, ::sf::Keyboard::BackSpace } };
	uint32_t mFrameNum;
};
}