#pragma once

namespace hvn3::math {

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

}