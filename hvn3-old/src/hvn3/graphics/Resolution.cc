#pragma once
#include "hvn3/graphics/Resolution.h"

namespace hvn3 {
	namespace Graphics {

		SizeI ResolutionToSize(Resolution resolution) {

			switch (resolution) {

			case Resolution::XGA:
				return SizeI(1024, 768);

			case Resolution::WXGA:
				return SizeI(1280, 800);

			case Resolution::WXGAPlus:
				return SizeI(1440, 900);

			case Resolution::FHD:
				return SizeI(1920, 1080);

			case Resolution::HD:
				return SizeI(1280, 720);

			case Resolution::VGA:
			default:
				return SizeI(640, 480);

			}

		}

	}
}