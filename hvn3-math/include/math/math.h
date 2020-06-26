#pragma once

namespace hvn3::math {

	const extern double Pi;

	template<typename T>
	T RadiansToDegrees(T radians) {

		return (radians * static_cast<T>(180)) / static_cast<T>(Pi);

	}

}