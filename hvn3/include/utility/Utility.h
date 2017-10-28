#pragma once
#include "math/MathUtils.h"
#include "utility/BitFlags.h"
#include <memory>
#include <vector>
#include <initializer_list>

namespace hvn3 {

	// Returns true if the item is contained in the collection.
	template <typename T>
	bool Contains(const std::vector<T>& items, const T& item) {

		return (std::find(items.begin(), items.end(), item) != items.end());

	}

	// Returns true if the item is contained in the collection.
	template <typename T>
	bool Contains(const std::initializer_list<T>& items, const T& item) {

		return (std::find(items.begin(), items.end(), item) != items.end());

	}

	template <typename T>
	T EaseTowards(T from, T to, T amount) {

		if (Math::Diff(from, to) <= amount)
			return to;

		if (from < to)
			from += amount;
		else
			from -= amount;

		return from;

	}

	// Converts seconds to the number of frames that would pass in that time.
	float SecondsToFrames(float fps, float sec);

	enum class Alignment {
		Left,
		Center,
		Right,
		Top,
		Bottom
	};
	ENABLE_BITFLAG_OPERATORS(Alignment);

	enum class SizeMode {
		Normal,
		Stretch,
		Zoom
	};

}