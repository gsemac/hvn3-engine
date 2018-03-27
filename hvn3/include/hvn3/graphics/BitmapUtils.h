#pragma once
#include "hvn3/graphics/Bitmap.h"

namespace hvn3 {
	namespace Graphics {

		void ConvertMaskToAlpha(Bitmap& bitmap, const Color& color);
		void SetAlphaFromBitmap(Bitmap& dest, Bitmap& src);
		void ConvertToGreyscale(Bitmap& bitmap);

	}
}