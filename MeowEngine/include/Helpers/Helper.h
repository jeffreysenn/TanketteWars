#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <random>
#include <limits>

namespace mw
{
namespace helper
{
namespace Vector
{
constexpr float PI = 3.14159265f;
template<typename T>
float getLength(const T& vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

template<typename T>
T normalize(T vector)
{
	float length = getLength(vector);
	if (length < 0.01)
		return T();
	return T(vector.x / length, vector.y / length);
}

inline float deg2rad(float deg)
{
	return deg / 180 * PI;
}

inline float rad2deg(float rad)
{
	return rad * 180 / PI;
}

inline ::sf::Vector2f deg2vec(float deg)
{
	return ::sf::Vector2f(cos(deg2rad(deg)), sin(deg2rad(deg)));
}
}

namespace Graphics
{
template<typename T>
void centreOrigin(T& object)
{
	::sf::FloatRect localBounds(object.getLocalBounds());
	object.setOrigin(localBounds.left + localBounds.width / 2,
					 localBounds.top + localBounds.height / 2);
}
}
namespace Number
{
template<typename T>
T getRandom(T min = ::std::numeric_limits<T>::min(), T max = ::std::numeric_limits<T>::max())
{
	::std::random_device dev;
	::std::mt19937 rng(dev());
	::std::uniform_int_distribution<T> dist(min, max);
	return dist(rng);
}
}

namespace Enum
{
// increment function for enum with continuous value
template<typename EnumClass>
void increment(EnumClass& e, int enumCount, int delta = 1, bool bLoopable = false)
{
	int current = static_cast<int>(e);
	int after = current + delta;

	if (bLoopable)
		after = (after % enumCount + enumCount) % enumCount;
	else
		after = after < 0 ? 0 : after > enumCount - 1 ? enumCount - 1 : after;

	e = static_cast<EnumClass>(after);
}

template<typename EnumClass>
const char* getText(const EnumClass& e, const char* EnumNames[])
{
	return EnumNames[static_cast<int>(e)];
}
}

}
}