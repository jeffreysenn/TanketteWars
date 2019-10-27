#pragma once
#include "StateBase.h"

#include "SFML/Graphics/Text.hpp"
namespace client
{

class TitleState : public StateBase
{
public:
	TitleState();
	~TitleState();

	virtual bool update(float deltaSeconds) override;
	virtual bool handleEvent(const ::sf::Event& event) override;
	virtual void draw() override;

private:
	void setupContinueText();
	void setupTitleText();
	void continueTextBlink(float deltaSeconds);

private:
	float mTextTime;
	::sf::Text mContinueText;

	::sf::Text mTitleText;

};


}