#include "ServerStateStack.h"
namespace server
{

ServerStateStack::ServerStateStack()
{
}

void ServerStateStack::processMessages()
{
	for (auto rit = mStack.rbegin(); rit != mStack.rend(); ++rit)
	{
		(*rit)->processMessages();
	}

	applyPendingChanges();
}

void ServerStateStack::packMessages()
{
	for (auto rit = mStack.rbegin(); rit != mStack.rend(); ++rit)
	{
		(*rit)->packMessages();
	}
}

}