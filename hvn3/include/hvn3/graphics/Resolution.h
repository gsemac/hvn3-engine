#pragma once
#include "hvn3/utility/Size.h"

namespace hvn3 {
	namespace Graphics {

		enum class Resolution {
			// 1024x768
			XGA,
			// 1280x800
			WXGA,
			// 1440x900
			WXGAPlus,
			// 640x480
			VGA,
			// 1280x720
			HD,
			// 1920x1080
			FHD,
			Fullscreen
		};

		// Returns a set of dimensions according to the given resolution.
		SizeI ResolutionToSize(Resolution resolution);

	}
}