#pragma once

#include <cmath>

namespace game::types
{

struct Vector
{
	float x, y;

	Vector()
		: x(0.f), y(0.f)
	{}

	Vector(const Vector& other)
		: x(other.x), y(other.y)
	{}

	Vector(int xVal, int yVal)
		: x(xVal), y(yVal)
	{}

	Vector(float xVal, float yVal)
		: x(xVal), y(yVal)
	{}

	Vector& normalize()
	{
		float length = std::sqrt(x * x + y * y);
		if (length != 0.0)
		{
			x /= length;
			y /= length;
		}

		return *this;
	}

	float sqDistanceTo(const Vector& other) const
	{
		float dx = x - other.x;
		float dy = y - other.y;
		return dx * dx + dy * dy;
	}

	Vector operator+(const Vector& other) const
	{
		return Vector(x + other.x, y + other.y);
	}

	Vector operator-(const Vector& other) const
	{
		return Vector(x - other.x, y - other.y);
	}

	Vector operator*(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vector& operator=(const Vector& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
};

}// namespace game::types