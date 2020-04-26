#pragma once

#include "hvn3/utility/BitFlags.h"

namespace hvn3::io {

	enum class WindowOptions {
		None = 0,
		UseOpenGL = 4,
		UseOpenGL3 = 8,
		UseForwardCompatibleOpenGL = 16,
		UseDirect3D = 32,
		Resizable = 64,
		Borderless = 128,
		FullscreenWindow = 256,
		AntiAliased = 512
	};

	HVN3_ENABLE_BITFLAGS(WindowOptions);

}