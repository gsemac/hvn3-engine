#pragma once

#include "utility/bitflags.h"

namespace hvn3::core {

	enum class EngineModules {
		// The core engine module, enabling basic features.
		Core = 1,
		// The I/O engine module, enabling keyboard and mouse input.
		IO = 2,
		Graphics = 4,
		Audio = 8,
		All = Core | IO | Graphics | Audio
	};

	HVN3_ENABLE_BITFLAG_OPERATORS(EngineModules)

}