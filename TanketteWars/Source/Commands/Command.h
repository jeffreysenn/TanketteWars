#pragma once
#include "CommandCategory.h"

#include <functional>
#include <cassert>

struct Command
{
	std::function<void(class Actor &, float)> action;
	CommandCategory category = CommandCategory::None;
};

template<typename MovableActor, typename Function>
std::function<void(Actor&, float)> derivedAction(Function fn)
{
	return [=](Actor &node, float deltaSeconds)
	{
		auto ptr = dynamic_cast<MovableActor*>(&node);
		assert(ptr);
		fn(*ptr, deltaSeconds);
	};
}