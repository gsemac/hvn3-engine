#pragma once
#include <algorithm>

namespace hvn3 {
	namespace Math {

		extern const long double Pi;
		extern const float FloatEpsilon;
		extern const double DoubleEpsilon;

		// Returns 1 if the number is greater than or equal to zero, or -1 if the number is less than zero.
		template <typename T>
		inline T Sign(T n) {

			return (n >= static_cast<T>(0) ? static_cast<T>(1) : static_cast<T>(-1));

		}

		// Returns 1 if the number is greater than zero, 0 if the number is equal to zero, or -1 if the number is less than zero.
		template <typename T>
		inline T Signum(T n) {

			return (n > static_cast<T>(0)) ? static_cast<T>(1) : ((n < static_cast<T>(0)) ? static_cast<T>(-1) : static_cast<T>(0));

		}

		// Returns the absolute difference between two numbers.
		template <typename T>
		inline T Diff(T n1, T n2) {

			return Abs(n1 - n2);

		}

		// Returns the absolute value of the given number.
		template <typename T>
		inline T Abs(T n) {

			return (std::abs)(n);

		}

		// Returns the maximum of two values.
		template <typename T>
		inline T Max(T n1, T n2) {

			return n1 > n2 ? n1 : n2;

		}

		// Returns the minimum of two values.
		template <typename T>
		inline T Min(T n1, T n2) {

			return n1 < n2 ? n1 : n2;

		}

		template <typename T>
		inline T Floor(T n) {

			return std::floor(n);

		}

		template <typename T>
		inline T Ceiling(T n) {

			return std::ceil(n);

		}
		// Returns the minimum of two values.
		//float Min(float a, float b);
		// Returns the given float rounded to the nearest integer.
		template <typename T>
		inline T Round(T n) {

			return (std::round)(n);

		}

		template <typename T>
		inline T Square(T n) {

			return (std::pow)(n, T(2));

		}

		// Forces a value to be between a minimum and maximum value.
		template <typename T>
		T Clamp(T value, T min, T max) {

			if (value > max)
				value = max;
			else if (value < min)
				value = min;
			return value;

		}

		// Returns true if n is within epsilon of zero.
		bool IsZero(float n, float epsilon);
		// Returns true if n is within epsilon of zero.
		bool IsZero(float n);
		// Returns true if n is within epsilon of zero.
		bool IsZero(double n, double epsilon);
		// Returns true if n is within epsilon of zero.
		bool IsZero(double n);

		// Returns true if the difference between n1 and n2 is less than epsilon.
		bool AreEqual(float n1, float n2, float epsilon);
		// Returns true if the difference between n1 and n2 is less than the default epsilon.
		bool AreEqual(float n1, float n2);
		// Returns true if the difference between n1 and n2 is less than epsilon.
		bool AreEqual(double n1, double n2, double epsilon);
		// Returns true if the difference between n1 and n2 is less than the default epsilon.
		bool AreEqual(double n1, double n2);

		bool IsGreaterThan(float n1, float n2, float epsilon);
		bool IsGreaterThan(float n1, float n2);
		bool IsGreaterThan(double n1, double n2, double epsilon);
		bool IsGreaterThan(double n1, double n2);

		bool IsLessThan(float n1, float n2, float epsilon);
		bool IsLessThan(float n1, float n2);
		bool IsLessThan(double n1, double n2, double epsilon);
		bool IsLessThan(double n1, double n2);

		// Returns the given degrees expressed in radians.
		float DegreesToRadians(float degrees);
		// Returns the given radians expressed in degrees.
		float RadiansToDegrees(float radians);

	}
}