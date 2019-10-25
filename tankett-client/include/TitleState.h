#pragma once
#include "ClientState.h"

#include "SFML/Graphics/Text.hpp"

class TitleState : public ClientState
{
public:
	TitleState();
	~TitleState();

	virtual bool update(float deltaSeconds) override;
	virtual bool handleEvent(const sf::Event &event) override;
	virtual void draw() override;

private:
	void setupContinueText();
	void setupTitleText();
	void continueTextBlink(float deltaSeconds);

private:
	float mTextTime;
	sf::Text mContinueText;

	sf::Text mTitleText;

};

