#pragma once

#include "utilities/bitflags.h"

namespace hvn3::graphics {

	enum class FontOptions {
		None = 0,
		Default = None,
		// Loads the font with no anti-aliasing.
		Monochrome = 1,
	};

	HVN3_ENABLE_BITFLAG_OPERATORS(FontOptions)

}