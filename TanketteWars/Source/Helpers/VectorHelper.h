#pragma once
#include <SFML/System/Vector2.hpp>
#define PI 3.14159265f

namespace VectorHelper
{
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

	inline sf::Vector2f deg2vec(float deg)
	{
		return sf::Vector2f(cos(deg2rad(deg)), sin(deg2rad(deg)));
	}
}