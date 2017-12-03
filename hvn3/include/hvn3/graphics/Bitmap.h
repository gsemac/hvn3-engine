#pragma once
#include "hvn3/core/SizeableBase.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Imaging.h"
#include "hvn3/io/File.h"

struct ALLEGRO_BITMAP;

namespace hvn3 {

	namespace Graphics {

		enum class BitmapFlags {
			Default = 1,
			MinLinear = 64,
			MagLinear = 128
		};
		ENABLE_BITFLAG_OPERATORS(BitmapFlags)

		struct BitmapData {
			BitmapData();
			unsigned char* Scan0;
			int PixelFormat;
			int Stride;
			int BytesPerPixel;
		};

		class Bitmap {

		public:
			Bitmap();
			Bitmap(int width, int height);
			Bitmap(const char* filename);
			Bitmap(const char* filename, const Color& alpha);
			Bitmap(ALLEGRO_BITMAP* bitmap, bool free = true);
			Bitmap(uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format);
			Bitmap(const Bitmap& other, const RectangleI& region);
			Bitmap(const Bitmap& other);
			Bitmap(Bitmap&& other);
			~Bitmap();

			Bitmap Clone() const;
			Bitmap Clone(const RectangleF& region) const;

			unsigned int Width() const;
			unsigned int Height() const;

			BitmapData Lock(IO::FileAccess access = IO::FileAccess::ReadWrite);
			BitmapData LockRegion(const RectangleI& region, IO::FileAccess access = IO::FileAccess::ReadWrite);
			void Unlock();
			bool IsLocked() const;

			void SetPixel(int x, int y, const Color& color);
			Color GetPixel(int x, int y) const;

			void ConvertMaskToAlpha(const Color& color);

			ALLEGRO_BITMAP* AlPtr() const;

			Bitmap& operator=(Bitmap& other);
			Bitmap& operator=(Bitmap&& other);
			explicit operator bool() const;

		private:
			ALLEGRO_BITMAP* _bitmap;
			bool _free;

			void _shallowCopy(const Bitmap& other);
			void _moveCopy(Bitmap& other);
			void _freeBitmap();

		};

		BitmapFlags NewBitmapFlags();
		void SetNewBitmapFlags(BitmapFlags flags);

	}

}