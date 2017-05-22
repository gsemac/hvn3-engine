#include "Utility.h"
#include "UTF8String.h"
#include "Font.h"
#include "String.h"
#include <algorithm>

namespace hvn3 {

	const float PI = std::acos(-1.0L);

	short Sign(float x) {

		return (x >= 0 ? 1 : -1);

	}
	short Signum(float x) {

		return (x > 0) ? 1 : ((x < 0) ? -1 : 0);

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
	float SecondsToFrames(float fps, float sec) {

		return fps * sec;

	}

}