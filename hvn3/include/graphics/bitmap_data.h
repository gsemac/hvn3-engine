#pragma once

#include "graphics/pixel_format.h"

namespace hvn3::graphics {

	struct BitmapData {

		unsigned char* Scan0;
		PixelFormat PixelFormat;
		int Stride;
		int BytesPerPixel;

		BitmapData();

	};

}