#pragma once

#include "graphics/bitmap_data.h"
#include "graphics/bitmap_options.h"
#include "graphics/color.h"
#include "io/file_access.h"
#include "io/image_format.h"
#include "math/rectangle.h"

#include <allegro5/allegro.h>

namespace hvn3::graphics {

	class Bitmap {

		typedef ALLEGRO_BITMAP underlying_bitmap_t;

	public:
		Bitmap(int width, int height);
		Bitmap(int width, int height, BitmapOptions options);
		Bitmap(const underlying_bitmap_t* bitmap, bool takeOwnership);

		Bitmap(const Bitmap& other);
		Bitmap(const Bitmap& other, const math::RectangleI& region);
		Bitmap(Bitmap&& other);

		~Bitmap();

		int Width() const;
		int Height() const;

		Bitmap Copy();
		Bitmap Copy(const math::RectangleI& region);

		BitmapData Lock(io::FileAccess access = io::FileAccess::ReadWrite);
		BitmapData LockRegion(const math::RectangleI& region, io::FileAccess access = io::FileAccess::ReadWrite);
		void Unlock();
		void IsLocked() const;

		void SetPixel(int x, int y, const Color& color);
		Color GetPixel(int x, int y) const;

		underlying_bitmap_t* GetUnderlyingBitmap(bool performPreWriteOperations);

		Bitmap& operator=(const Bitmap& other);
		Bitmap& operator=(Bitmap&& other);

		explicit operator bool() const;

		static Bitmap FromBuffer(const uint8_t* buffer, size_t buffer_size, io::ImageFormat format);
		static Bitmap FromFile(const std::string& file);
		static Bitmap FromFile(const std::string& file, const Color& alpha_color);

	private:
		void InitializeAllegro();

	};

	bool operator==(const Bitmap& lhs, const Bitmap& rhs);
	bool operator!=(const Bitmap& lhs, const Bitmap& rhs);

}