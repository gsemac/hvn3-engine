#include "MathUtils.h"

namespace hvn3 {
	namespace Math {

		const long double Pi = std::acos(-1.0L);

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

			return (degrees * (Pi / 180.0f));

		}
		float RadiansToDegrees(float radians) {

			return ((radians * 180.0f) / Pi);

		}


	}
}