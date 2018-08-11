#pragma once
#include "hvn3/math/MathUtils.h"
#include "hvn3/utility/BitFlags.h"
#include <memory>
#include <vector>
#include <initializer_list>

namespace hvn3 {

	// Returns true if the item is contained in the collection.
	template <typename T>
	bool Contains(std::vector<T> items, const T& item) {

		return (std::find(items.begin(), items.end(), item) != items.end());

	}

	// Returns true if the item is contained in the collection.
	template <typename T>
	bool Contains(std::initializer_list<T> items, const T& item) {

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
		// Aligns horizontally to the left.
		Left = 1,
		// Aligns horizontally to the center.
		Center = 2,
		// Aligns horizontally to the right.
		Right = 4,
		// Aligns vertically to the top.
		Top = 8,
		// Aligns vertically to the middle.
		Middle = 16,
		// Aligns vertically to the bottom.
		Bottom = 32
	};
	ENABLE_BITFLAG_OPERATORS(Alignment)

	enum class SizeMode {
		Normal,
		Stretch,
		Zoom
	};

}