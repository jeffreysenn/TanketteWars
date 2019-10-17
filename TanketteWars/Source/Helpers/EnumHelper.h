#pragma once


namespace EnumHelper
{
	// increment function for enum with continuous value
	template<typename EnumClass>
	void increment(EnumClass &e, int enumCount, int delta = 1, bool bLoopable = false)
	{
		int current = static_cast<int>(e);
		int after = current + delta;

		if (bLoopable)
			after = (after%enumCount + enumCount) % enumCount;
		else
			after = after < 0 ? 0 : after > enumCount - 1 ? enumCount - 1 : after;

		e = static_cast<EnumClass>(after);
	}

	template<typename EnumClass>
	const char* getText(const EnumClass &e, const char* EnumNames[])
	{
		return EnumNames[static_cast<int>(e)];
	}
}
