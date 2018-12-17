#pragma once

#include <algorithm>
#include <cassert>

namespace hvn3 {
	namespace Math {

		extern const long double Pi;

		template <typename T>
		struct Epsilon {
			static const T value = static_cast<T>(0);
		};
		template <>
		struct Epsilon<float> {
			static const float value;
		};
		template <>
		struct Epsilon<double> {
			static const double value;
		};

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

		// Returns the minimum of all given values.
		template <typename T, typename... Args>
		T Min(T n, Args... args) {

			T m = Min(args...);

			return m < n ? m : n;

		}

		template <typename T>
		inline T Floor(T n) {

			return std::floor(n);

		}

		template <typename T>
		inline T Floor(T n, T multiple) {

			return (std::floor)(n / multiple) * multiple;

		}

		template <typename T>
		inline T Ceiling(T n) {

			return std::ceil(n);

		}

		template <typename T>
		inline T Ceiling(T n, T multiple) {

			if (multiple == static_cast<T>(0))
				return n;

			T r = Mod((std::abs)(n), multiple);

			if (r == static_cast<T>(0))
				return n;

			if (n < static_cast<T>(0))
				return -((std::abs)(n) - r);
			else
				return n + multiple - r;

		}

		// Returns the given value rounded to the nearest integer.
		template <typename T>
		inline T Round(T n) {
			return (std::round)(n);
		}

		// Rounds the given value to the nearest multiple of the second value.
		template <typename T>
		inline T Round(T n, T multiple) {

			if (multiple == static_cast<T>(0))
				return n;

			return (std::floor)((n + multiple / static_cast<T>(2)) / multiple) * multiple;

		}

		template <typename T>
		inline T Square(T n) {

			return (std::pow)(n, T(2));

		}

		template <typename T>
		typename std::enable_if<std::is_integral<T>::value, T>::type Mod(T lhs, T rhs) {

			return lhs % rhs;

		}

		template <typename T>
		typename std::enable_if<std::is_floating_point<T>::value, T>::type Mod(T lhs, T rhs) {

			return (std::fmod)(lhs, rhs);

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
		template <typename T>
		bool IsZero(T n, T epsilon) {

			return (n < epsilon && n > -epsilon);

		}
		// Returns true if n is within epsilon of zero.
		template <typename T>
		bool IsZero(T n) {

			return IsZero(n, Epsilon<T>::value);

		}

		template<typename T>
		bool IsNegative(T n) {

			return n < static_cast<T>(0);

		}

		// Returns true if the difference between n1 and n2 is less than epsilon.
		template <typename T>
		bool AreEqual(T n1, T n2, T epsilon) {

			return (Diff(n1, n2) < epsilon);

		}
		// Returns true if the difference between n1 and n2 is less than the default epsilon.
		template <typename T>
		bool AreEqual(T n1, T n2) {

			return AreEqual(n1, n2, Epsilon<T>::value);

		}

		template <typename T>
		bool IsGreaterThan(T n1, T n2, T epsilon) {

			return n1 > n2 && Diff(n1, n2) >= epsilon;

		}
		template <typename T>
		bool IsGreaterThan(T n1, T n2) {

			return IsGreaterThan(n1, n2, Epsilon<T>::value);

		}

		template <typename T>
		bool IsLessThan(T n1, T n2, T epsilon) {

			return n1 < n2 && Diff(n1, n2) >= epsilon;

		}
		template <typename T>
		bool IsLessThan(T n1, T n2) {

			return IsLessThan(n1, n2, Epsilon<T>::value);

		}

		// Returns the given degrees expressed in radians.
		template <typename T>
		T DegreesToRadians(T degrees) {

			return (degrees * (static_cast<T>(Pi) / static_cast<T>(180)));

		}
		// Returns the given radians expressed in degrees.
		template <typename T>
		T RadiansToDegrees(T radians) {

			return ((radians * static_cast<T>(180)) / static_cast<T>(Pi));

		}

		// Returns an angle in the given range, or a default of 0-360.
		template <typename T>
		T NormalizeAngle(T degrees, T min = 0, T max = 360) {

			T range = max - min;
			T offset = degrees - min;

			return (offset - ((std::floor)(offset / range) * range)) + min;

		}
		// Returns the smallest difference between two angles.
		template <typename T>
		T AngleDifference(T from, T to) {

			T diff = to - from;
			while (diff < static_cast<T>(-180))
				diff += static_cast<T>(360);
			while (diff > static_cast<T>(180))
				diff -= static_cast<T>(360);

			return diff;

		}
		// Returns the quadrant that the angle resides in, from 1-4.
		template <typename T>
		int AngleQuadrant(T degrees) {

			degrees = NormalizeAngle(degrees);

			if (degrees <= 90)
				return 1;
			if (degrees <= 180)
				return 2;
			if (degrees <= 270)
				return 3;
			return 4;

		}
		// Returns the given angle mirrored over the X axis.
		template <typename T>
		T AngleFlipX(T degrees) {

			return static_cast<T>(540) - degrees;

		}
		// Returns the given angle mirrored over the Y axis.
		template <typename T>
		T AngleFlipY(T degrees) {

			return static_cast<T>(360) - degrees;

		}

		// Performs modulo with floored division.
		template<typename T, typename N>
		T ModFloor(T a, N n) {

			return ((a % n) + n) % n;

		}

		// Returns the cosine of an angle, given in degrees.
		template<typename T>
		T Cosine(T degrees) {

			T rad = DegreesToRadians(degrees);

			return std::cos(rad);

		}

		// Increments or decrements a value in the direction of its sign.
		template <typename T>
		T SignedIncrement(T n) {

			if (n < 0)
				return n - static_cast<T>(1);
			else
				return n + -static_cast<T>(1);

		}

		// Increments or decrements a value in the direction of the sign of the second value.
		template <typename T>
		T SignedIncrement(T n, T sign) {

			if (sign < 0)
				return n - static_cast<T>(1);
			else
				return n + -static_cast<T>(1);

		}

		// Increments a value by the given amount and wraps the result in the range [min, max].
		template <typename T>
		T WrappedIncrement(T n, T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max(), T amount = 1) {

			// #todo This could be performed far more efficiently for the cases where abs(amount) > 1.

			assert(n >= min);
			assert(n <= max);
			assert(min < max);

			for (T i = 0; i < Abs(amount); i += 1) {

				if (n == max && Sign(amount) > 0)
					n = min;
				else if (n == min && Sign(amount) < 0)
					n = max;
				else
					n += Sign(amount);

			}

			return n;

		}

	}
}