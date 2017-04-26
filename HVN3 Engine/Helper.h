#ifndef __HELPER_H
#define __HELPER_H
#include <memory>
#include <vector>
#include <initializer_list>
#include "BitFlags.h"

class Utf8String;
class Font;

extern const float PI;

// Returns the sign of a given number (1 for all numbers greater than or equal to 0; -1 otherwise). 
short Sign(float);
// Returns the sign of a given number (1, 0, or -1). 
short Signum(float);

// Returns the absolute difference between two numbers.
float AbsDiff(float a, float b);
// Returns the absolute difference between two numbers.
int AbsDiff(int a, int b);
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

// Returns a collection of Utf8String, split into lines such that each line fits the specified width.
std::vector<std::shared_ptr<Utf8String>> Fit(const std::shared_ptr<Utf8String>&, const Font&, float);

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
float Clamp(float value, float min, float max);

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

#endif