#include "StateBase.h"
#include "Context.h"
namespace client
{

StateBase::StateBase()
{
}

StateBase::~StateBase()
{
}

Context& StateBase::getContext() const
{
	return Context::getInstance();
}

::sf::RenderWindow& StateBase::getRenderWindow() const
{
	return *Context::getInstance().window;
}

}