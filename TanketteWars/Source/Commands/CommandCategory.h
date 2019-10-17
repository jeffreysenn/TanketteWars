#pragma once

enum class CommandCategory : unsigned int
{
	None = 0,
	Scene = 1,
	Avatar = 1 << 1,
	Enemy = 1 << 2,
	Tank0 = 1 << 3,
	Tank1 = 1 << 4,
	Tank2 = 1 << 5,
	Tank3 = 1 << 6,
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