#pragma once

#include <memory>

namespace sf
{
	class Event;
	class RenderWindow;
}

class ClientState
{
public:
	explicit ClientState();
	virtual ~ClientState();

	virtual bool update(float deltaSeconds) { return true; }
	virtual bool handleEvent(const sf::Event &event) { return true; }
	virtual void draw() {}

protected:
	struct ClientContext& getContext() const;
	sf::RenderWindow& getRenderWindow() const;
};

