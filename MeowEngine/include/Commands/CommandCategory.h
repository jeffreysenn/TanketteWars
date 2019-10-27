#pragma once
namespace mw
{
enum class CommandCategory : unsigned int
{
	None = 0 << 0,
	Tank0 = 1 << 1,
	Tank1 = 1 << 2,
	Tank2 = 1 << 3,
	Tank3 = 1 << 4,
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