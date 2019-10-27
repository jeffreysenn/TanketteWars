#pragma once

#include <memory>

namespace sf
{
class Event;
class RenderWindow;
}
namespace client
{

class StateBase
{
public:
	explicit StateBase();
	virtual ~StateBase();

	virtual bool update(float deltaSeconds) { return true; }
	virtual bool handleEvent(const ::sf::Event& event) { return true; }
	virtual void draw() {}

protected:
	struct Context& getContext() const;
	::sf::RenderWindow& getRenderWindow() const;
};


}