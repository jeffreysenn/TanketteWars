#pragma once
#include <stdint.h>

namespace mw
{
enum class CommandCategory : uint32_t
{
	None = 0,
	Tank0 = 1 << 0,
	Tank1 = 1 << 1,
	Tank2 = 1 << 2,
	Tank3 = 1 << 3,
	TankManager = 1 << 4,
};

inline CommandCategory operator | (CommandCategory a, CommandCategory b)
{
	return static_cast<CommandCategory>(
		static_cast<unsigned int>(a)
		| static_cast<unsigned int>(b));
}

inline CommandCategory operator & (CommandCategory a, CommandCategory b)
{
	return static_cast<CommandCategory>(
		static_cast<unsigned int>(a)
		& static_cast<unsigned int>(b));
}
}