#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Imaging.h"
#include "hvn3/io/File.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/utility/Size.h"
#include <memory>
#include <string>

struct ALLEGRO_BITMAP;

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}

	namespace Graphics {

		enum class BitmapFlags {
			Default = 1,
			AllegroDefault = 1,
			MinLinear = 64,
			MagLinear = 128,
			AntiAlias = MinLinear | MagLinear
		};
		HVN3_ENABLE_BITFLAGS(BitmapFlags)

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
			Bitmap(const SizeI& size);
			Bitmap(const SizeI& size, BitmapFlags flags);
			Bitmap(int width, int height);
			Bitmap(int width, int height, BitmapFlags flags);
			Bitmap(ALLEGRO_BITMAP* bitmap, bool takeOwnership);

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
			
			void Save(const std::string& filename) const;

			Bitmap& operator=(const Bitmap& other);
			Bitmap& operator=(Bitmap&& other);
			bool operator==(const Bitmap& other) const;
			bool operator!=(const Bitmap& other) const;
			explicit operator bool() const;

			static Bitmap FromBuffer(const uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format);
			static Bitmap FromFile(const std::string& file);
			static Bitmap FromFile(const std::string& file, const Color& alpha_color);

			static BitmapFlags DefaultBitmapFlags();
			static void SetDefaultBitmapFlags(BitmapFlags flags);
			static void AddDefaultBitmapFlags(BitmapFlags flags);
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