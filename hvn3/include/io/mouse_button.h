#pragma once

#include "core/bitflags.h"

namespace hvn3::io {

	enum class MouseButton {
		None = 0,
		Left = 1,
		Right = 2,
		Middle = 4
	};

	HVN3_ENABLE_BITFLAG_OPERATORS(MouseButton);

}