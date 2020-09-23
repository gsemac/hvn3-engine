#pragma once

#include "math/epsilon.h"

#include <cassert>
#include <cmath>
#include <type_traits>

namespace hvn3::math {

	const extern double Pi;

	template <typename T>
	typename std::enable_if<std::is_integral<T>::value, T>::type Mod(T lhs, T rhs) {

		return lhs % rhs;

	}
	template <typename T>
	typename std::enable_if<std::is_floating_point<T>::value, T>::type Mod(T lhs, T rhs) {

		return std::fmod(lhs, rhs);

	}

	template<typename T>
	T RadiansToDegrees(T radians) {

		return (radians * static_cast<T>(180)) / static_cast<T>(Pi);

	}
	template <typename T>
	T DegreesToRadians(T degrees) {

		return (degrees * (static_cast<T>(Pi) / static_cast<T>(180)));

	}

	template<typename T>
	T Clamp(T value, T min, T max) {

		if (value > max)
			value = max;
		else if (value < min)
			value = min;

		return value;

	}
	template<typename T>
	T Wrap(T value, T min, T max) {

		assert(min < max);

		return Mod(value, max - min) + min;

	}

	template<typename T>
	bool IsZero(T value) {

		return IsZero(value, Epsilon<T>::value);

	}
	template <typename T>
	bool IsZero(T value, T epsilon) {

		return AreEqual(value, static_cast<T>(0), epsilon);

	}

	template<typename T>
	bool AreEqual(T lhs, T rhs) {

		return AreEqual(lhs, rhs, Epsilon<T>::value);

	}
	template <typename T>
	bool AreEqual(T lhs, T rhs, T epsilon) {

		return std::abs(lhs - rhs) < epsilon;

	}

}