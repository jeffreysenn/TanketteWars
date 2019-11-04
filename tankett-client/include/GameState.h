#pragma once
#include "StateBase.h"
#include "World.h"
#include "Rendering/Renderer.h"
#include "Input/Input.h"
#include "PlayerController.h"
#include "ScoreBoard.h"

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
	void handleInput();
	void updateScoreboard();
	void updateCooldownText();
	virtual void draw() override;
	virtual bool handleEvent(const ::sf::Event& event) override;

private:
	void processReceivedMessages();
	void checkNewRemote(::tankett::message_server_to_client* msgS2C);
	void updateState(::tankett::message_server_to_client* msgS2C);
	void validateInputPrediction(const ::tankett::PlayerState& state, uint32_t inputNum);
	void syncBulletState(const tankett::PlayerState& state, ::tankett::PlayerController& controller);
	void packInput();
	void pushInputMessage(::tankett::PlayerController::TankInput& inputValue, uint32_t inputNum);

private:
	World mWorld;
	::std::vector<::std::unique_ptr<::tankett::PlayerController>> mPlayerControllers;
	::tankett::PlayerController* mLocalController{};
	::std::map<uint32_t, ::tankett::PlayerState> mPredictedStates;
	::std::map<uint32_t, ::tankett::PlayerState> mRemoteStates;
	::mw::Renderer mRenderer;
	::mw::Input::InputCollection mPauseInputs{
		{ Input::Type::Keyboard, ::sf::Keyboard::Escape },
		{ Input::Type::Keyboard, ::sf::Keyboard::BackSpace } };
	uint32_t mFrameNum;
	::sf::Text mRoundTimerText;
	::sf::Text mCooldownText;
	ScoreBoard mScoreBoard;
};
}