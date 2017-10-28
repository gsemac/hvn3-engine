#pragma once
#include "graphics/Bitmap.h"

namespace hvn3 {

	class Background {

	private:
		Drawing::Bitmap __bitmap;

	public:
		Background(const char* filename);
		Background(Background&& other);
		unsigned int Width() const;
		unsigned int Height() const;

		// Returns a reference to the underlying Bitmap.
		const Drawing::Bitmap& Bitmap() const;

	};

}