#include "Helper.h"
#include "UTF8String.h"
#include "Font.h"
#include "String.h"
#include <algorithm>

const float PI = std::acos(-1.0L);

short Sign(float x) {

	return (x >= 0 ? 1 : -1);

}
short Signum(float x) {

	return (x > 0) ? 1 : ((x < 0) ? -1 : 0);

}

float AbsDiff(float a, float b) {

	return (std::fabs)(a - b);

}
int AbsDiff(int a, int b) {

	return (std::abs)(a - b);

}

//float Max(float a, float b) {
//
//	return (std::fmax)(a, b);
//
//}
//float Max(int a, int b) {
//
//	return (std::max)(a, b);
//
//}
//float Min(float a, float b) {
//
//	return (std::fmin)(a, b);
//
//}
int Round(float n) {

	return (std::roundf)(n);

}
bool IsZero(float n, float epsilon) {

	return (n < epsilon && n > -epsilon);

}

float DegreesToRadians(float degrees) {

	return (degrees * (PI / 180.0F));

}
float RadiansToDegrees(float radians) {

	return ((radians * 180.0f) / PI);

}

std::vector<std::shared_ptr<String>> Fit(const std::shared_ptr<String>& ustr, const Font& font, float width) {

	// Initialize return structure.
	std::vector<std::shared_ptr<String>> resized_lines;

	// Initialize other variables.
	int last_split_index = 0;
	int last_space_index = 0;
	int next_space_index = 0;

	while (1) {
		next_space_index = ustr->IndexOf(' ', last_space_index + 1);
		if (next_space_index == -1) {
			// We've reached the end of the line. Cut off the last word if we need to (and can), then add both lines to
			// the list and exit the loop.
			String tmp(ustr->Substring(last_split_index, (int)ustr->Length()));
			if (tmp.Width(font) > width) {
				resized_lines.push_back(std::make_shared<String>(ustr->Substring(last_split_index, last_space_index + 1)));
				last_split_index = last_space_index + 1;
			}
			resized_lines.push_back(std::make_shared<String>(ustr->Substring(last_split_index, (int)ustr->Length())));
			break;
		}
		else {

			// Check the width of the line up to this space. If it's too long, go back a space and add that (smallest fit).
			String tmp(ustr->Substring(last_split_index, next_space_index));
			if (tmp.Width(font) > width) {
				resized_lines.push_back(std::make_shared<String>(ustr->Substring(last_split_index, last_space_index + 1)));
				last_split_index = last_space_index + 1;
			}
			last_space_index = next_space_index;

		}

	}

	// Return the result.
	return resized_lines;

}

float Clamp(float value, float min, float max) {

	if (value > max)
		value = max;
	else if (value < min)
		value = min;
	return value;

}

float SecondsToFrames(float fps, float sec) {

	return fps * sec;

}
