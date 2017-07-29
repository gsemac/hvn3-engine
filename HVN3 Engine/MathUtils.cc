#include "MathUtils.h"

namespace hvn3 {
	namespace Math {

		const long double Pi = std::acos(-1.0L);
		const float FloatEpsilon = 0.01f;
		const double DoubleEpsilon = 0.01;

		short Sign(float x) {

			return (x >= 0 ? 1 : -1);

		}
		short Signum(float x) {

			return (x > 0) ? 1 : ((x < 0) ? -1 : 0);

		}

		bool IsZero(float n, float epsilon) {

			return (n < epsilon && n > -epsilon);

		}
		bool IsZero(float n) {

			return IsZero(n, FloatEpsilon);

		}
		bool IsZero(double n, double epsilon) {

			return (n < epsilon && n > -epsilon);

		}
		bool IsZero(double n) {

			return IsZero(n, DoubleEpsilon);

		}

		bool AreEqual(float n1, float n2, float epsilon) {

			return (Diff(n1, n2) < epsilon);

		}
		bool AreEqual(float n1, float n2) {

			return AreEqual(n1, n2, FloatEpsilon);

		}
		bool AreEqual(double n1, double n2, double epsilon) {

			return (Diff(n1, n2) < epsilon);

		}
		bool AreEqual(double n1, double n2) {

			return AreEqual(n1, n2, DoubleEpsilon);

		}

		bool IsGreaterThan(float n1, float n2, float epsilon) {

			return n1 > n2 && Diff(n1, n2) < epsilon;

		}
		bool IsGreaterThan(float n1, float n2) {

			return IsGreaterThan(n1, n2, FloatEpsilon);

		}
		bool IsGreaterThan(double n1, double n2, double epsilon) {

			return n1 > n2 && Diff(n1, n2) < epsilon;

		}
		bool IsGreaterThan(double n1, double n2) {

			return IsGreaterThan(n1, n2, DoubleEpsilon);

		}

		bool IsLessThan(float n1, float n2, float epsilon) {

			return n1 < n2 && Diff(n1, n2) < epsilon;

		}
		bool IsLessThan(float n1, float n2) {

			return IsLessThan(n1, n2, FloatEpsilon);

		}
		bool IsLessThan(double n1, double n2, double epsilon) {

			return n1 < n2 && Diff(n1, n2) < epsilon;

		}
		bool IsLessThan(double n1, double n2) {

			return IsLessThan(n1, n2, DoubleEpsilon);

		}

		float DegreesToRadians(float degrees) {

			return (degrees * (Pi / 180.0f));

		}
		float RadiansToDegrees(float radians) {

			return ((radians * 180.0f) / Pi);

		}


	}
}