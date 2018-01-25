#pragma once
#include "hvn3/graphics/Color.h"
#include <memory>
#include <string>

struct ALLEGRO_BITMAP;

namespace hvn3 {
	namespace Graphics {

		class CopyOnWriteBitmap {
			
			friend class Graphics;

			typedef std::shared_ptr<ALLEGRO_BITMAP> bitmap_ptr_type;

		public:
			CopyOnWriteBitmap();
			CopyOnWriteBitmap(int width, int height);
			CopyOnWriteBitmap(const std::string& file);
			CopyOnWriteBitmap(const std::string& file, const Color& alpha_color);
			CopyOnWriteBitmap(ALLEGRO_BITMAP* bitmap, bool managed = true);

			CopyOnWriteBitmap(const CopyOnWriteBitmap& other);
			CopyOnWriteBitmap(const CopyOnWriteBitmap& other, const RectangleI& region);
			CopyOnWriteBitmap(CopyOnWriteBitmap&& other);

			~CopyOnWriteBitmap();

			CopyOnWriteBitmap Copy() const;
			CopyOnWriteBitmap Copy(const RectangleI& region) const;

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

			CopyOnWriteBitmap& operator=(CopyOnWriteBitmap& other);
			CopyOnWriteBitmap& operator=(CopyOnWriteBitmap&& other);
			explicit operator bool() const;

			static CopyOnWriteBitmap FromBuffer(uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format);

		private:
			bitmap_ptr_type _src_bitmap;
			ALLEGRO_BITMAP* _sub_bitmap;
			bool _managed;

			bool _copy_required() const;
			ALLEGRO_BITMAP* _get_bitmap_ptr() const;
			void _perform_pre_write_operations();

			void _copy_assign(const CopyOnWriteBitmap& other);
			void _move_assign(CopyOnWriteBitmap& other);

		};

	}
}