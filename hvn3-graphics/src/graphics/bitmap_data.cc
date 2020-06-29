#include "graphics/bitmap_data.h"

namespace hvn3::graphics {

	BitmapData::BitmapData() {

		Scan0 = nullptr;
		PixelFormat = PixelFormat::Any;
		Stride = 0;
		BytesPerPixel = 0;

	}

}