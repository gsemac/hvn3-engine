#pragma once

namespace hvn3::graphics {

	struct BitmapData {

		unsigned char* Scan0;
		int PixelFormat;
		int Stride;
		int BytesPerPixel;

		BitmapData();

	};

}