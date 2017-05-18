#pragma once
#include <memory>
#include <vector>
#include <initializer_list>
#include "BitFlags.h"

class String;
class Font;

extern const float PI;

// Returns the sign of a given number (1 for all numbers greater than or equal to 0; -1 otherwise). 
short Sign(float);
// Returns the sign of a given number (1, 0, or -1). 
short Signum(float);

// Returns the absolute difference between two numbers.
template <typename T>
inline T Diff(T a, T b) {

	return Abs(a - b);

}
template <typename T>
inline T Abs(T value) {

	return (std::abs)(value);

}
// Returns the maximum of two values.
template <typename T>
inline T Max(T a, T b) {

	return a > b ? a : b;

}
// Returns the maximum of two values.
//float Max(float a, float b);
// Returns the maximum of two values.
//float Max(int a, int b);
// Returns the minimum of two values.
template <typename T>
inline T Min(T a, T b) {

	return a < b ? a : b;

}
// Returns the minimum of two values.
//float Min(float a, float b);
// Returns the given float rounded to the nearest integer.
int Round(float n);
// Returns true if n is within epsilon of 0.
bool IsZero(float n, float epsilon);

// Converts from degrees to radians.
float DegreesToRadians(float degrees);
// Converts from radians to degrees.
float RadiansToDegrees(float radians);

// Returns true if the item is contained in the collection.
template <typename T>
bool Contains(const std::vector<T>& items, const T& item) {

	return (std::find(items.begin(), items.end(), item) != items.end());

}

// Returns true if the item is contained in the collection.
template <typename T>
bool Contains(const std::initializer_list<T>& items, const T& item) {

	return (std::find(items.begin(), items.end(), item) != items.end());

}

// Forces a value to be between a minimum and maximum value.
template <typename T>
float Clamp(T value, T min, T max) {

	if (value > max)
		value = max;
	else if (value < min)
		value = min;
	return value;

}

// Converts seconds to the number of frames that would pass in that time.
float SecondsToFrames(float fps, float sec);

enum class Alignment {
	Left,
	Center,
	Right,
	Top,
	Bottom
};
ENABLE_BITFLAG_OPERATORS(Alignment);

enum class SizeMode {
	Normal,
	Stretch,
	Zoom
};