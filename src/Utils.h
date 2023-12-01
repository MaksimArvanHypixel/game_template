#pragma once
#include <random>
#include <raylib.h>
#include <Vector.h>

namespace utils
{

float RandomFloatInRange(float min, float max)
{
	std::random_device rd; // Obtain a random number from hardware
	std::mt19937 gen(rd());// Seed the generator

	std::uniform_real_distribution<float> dis(min, max);// Define the range

	return dis(gen);// Generate a random float within the specified range
}

game::types::Vector GetRandomPositionInRectangle(const Rectangle& rectangle)
{
	return {RandomFloatInRange(rectangle.x, rectangle.width),
			RandomFloatInRange(rectangle.y, rectangle.height)};
}

game::types::Vector GetRandomDirection()
{
	game::types::Vector vector{
			RandomFloatInRange(0, 1),
			RandomFloatInRange(0, 1)};

	vector.normalize();

	return vector;
}

float SquareOf(float value)
{
	return value * value;
}

bool IsPositionInsideOfRectangleX(const Rectangle& rectangle, const game::types::Vector& position)
{
	return position.x >= rectangle.x && position.x <= (rectangle.x + rectangle.width);
};

bool IsPositionInsideOfRectangleY(const Rectangle& rectangle, const game::types::Vector& position)
{
	return position.y >= rectangle.y && position.y <= (rectangle.y + rectangle.height);
};

bool IsPositionInsideOfRectangle(const Rectangle& rectangle, const game::types::Vector& position)
{
	return IsPositionInsideOfRectangleX(rectangle, position) 
		&& IsPositionInsideOfRectangleY(rectangle, position);
}


}// namespace game