#pragma once

#include "core/bitflags.h"

namespace hvn3::graphics {

	enum class Alignment {
		// Aligns horizontally to the left.
		Left = 1,
		// Aligns horizontally to the center.
		HorizontalCenter = 2,
		// Aligns horizontally to the right.
		Right = 4,
		// Aligns vertically to the top.
		Top = 8,
		// Aligns vertically to the middle.
		VerticalCenter = 16,
		// Aligns vertically to the bottom.
		Bottom = 32
	};

	HVN3_ENABLE_BITFLAG_OPERATORS(Alignment);

}