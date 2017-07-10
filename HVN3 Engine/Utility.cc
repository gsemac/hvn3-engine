#include "Utility.h"
#include "UTF8String.h"
#include "Font.h"
#include "String.h"
#include <algorithm>

namespace hvn3 {

	const long double PI = std::acos(-1.0L);

	short Sign(float x) {

		return (x >= 0 ? 1 : -1);

	}
	short Signum(float x) {

		return (x > 0) ? 1 : ((x < 0) ? -1 : 0);

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