#pragma once
#include "State.h"
#include "../Input/InputCollections.h"
#include "../Helpers/GraphicsHelper.h"
#include "../Helpers/EnumHelper.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <string>

template<typename EnumClass>
class MenuTemplate : public State
{
public:
	MenuTemplate(StateStack &stateStack, const Context &context,
				 const char* optionNames[], int optionCount);
	virtual ~MenuTemplate(){}


	virtual bool update(float deltaSeconds) override;
	virtual bool handleEvent(const sf::Event& event) override;
	virtual void draw() override;

	EnumClass getCurrentOption() const { return mCurrentOption; }

protected:
	virtual void updateOptions();

	virtual void setupOptions();
	virtual void setupOptionsText(sf::Text& text, const EnumClass &option);
	virtual void setupOptionsTextLocation(sf::Text &text, const EnumClass &option);

	void drawOptions();

	virtual void handleConfirmInput();

private:
	typedef std::unique_ptr<sf::Text> TextPtr;
	std::map<EnumClass, TextPtr> mOptionMap;
	int mOptionCount;
	const char** mOptionNames;
	EnumClass mCurrentOption;
};

#include "MenuTemplate.inl"

