#pragma once

#include "core/bitflags.h"

namespace hvn3::graphics {

	enum class BitmapOptions {
		None = 0,
		Default = 1,
		MinLinear = 2,
		MagLinear = 4,
		AntiAlias = MinLinear | MagLinear
	};

	HVN3_ENABLE_BITFLAG_OPERATORS(BitmapOptions);

}