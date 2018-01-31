#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Imaging.h"
#include "hvn3/io/File.h"
#include "hvn3/math/Rectangle.h"
#include <memory>
#include <string>

struct ALLEGRO_BITMAP;

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}

	namespace Graphics {

		enum class BitmapFlags {
			AllegroDefault = 1,
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
			
			friend class Graphics;
			friend class System::AllegroAdapter;

			typedef std::shared_ptr<ALLEGRO_BITMAP> bitmap_ptr_type;

		public:
			Bitmap();
			Bitmap(int width, int height);
			Bitmap(ALLEGRO_BITMAP* bitmap, bool managed = true);

			Bitmap(const Bitmap& other);
			Bitmap(const Bitmap& other, const RectangleI& region);
			Bitmap(Bitmap&& other);

			~Bitmap();

			Bitmap Copy() const;
			Bitmap Copy(const RectangleI& region) const;

			int Width() const;
			int Height() const;

			bool IsVideoBitmap() const;
			void ConvertToVideoBitmap();

			BitmapData Lock(IO::FileAccess access = IO::FileAccess::ReadWrite);
			BitmapData LockRegion(const RectangleI& region, IO::FileAccess access = IO::FileAccess::ReadWrite);
			void Unlock();
			bool IsLocked() const;

			void SetPixel(int x, int y, const Color& color);
			Color GetPixel(int x, int y) const;

			void ConvertMaskToAlpha(const Color& color);

			Bitmap& operator=(const Bitmap& other);
			Bitmap& operator=(Bitmap&& other);
			explicit operator bool() const;

			static Bitmap FromBuffer(uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format);
			static Bitmap FromFile(const std::string& file);
			static Bitmap FromFile(const std::string& file, const Color& alpha_color);

			static BitmapFlags NewBitmapFlags();
			static void SetNewBitmapFlags(BitmapFlags flags);
			static void ConvertMemoryBitmapsToVideoBitmaps();

		private:
			bitmap_ptr_type _src_bitmap;
			ALLEGRO_BITMAP* _sub_bitmap;
			bool _managed;

			bool _copy_required() const;
			ALLEGRO_BITMAP* _get_bitmap_ptr() const;
			void _perform_pre_write_operations();

			void _copy_assign(const Bitmap& other);
			void _move_assign(Bitmap& other);

		};

	}
}