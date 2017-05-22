#include "Bitmap.h"
#include "Graphics.h"
#include "FrameworkAdapter.h"

namespace hvn3 {

	namespace Drawing {

		Bitmap::Bitmap() {

			__bmp = nullptr;
			__free = false;

		}
		Bitmap::Bitmap(int width, int height) {

			// If either the width or height is 0, do not allocate any memory for the bitmap.
			if (width == 0 || height == 0) {
				__bmp = nullptr;
				__free = false;
				return;
			}

			// Otherwise, allocate memory for the bitmap.
			__bmp = al_create_bitmap(width, height);
			__free = true;

		}
		Bitmap::Bitmap(const char* filename) {

			__bmp = al_load_bitmap(filename);
			__free = true;

		}
		Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap, bool free) {

			__bmp = bitmap;
			__free = free;

		}
		// Creates a sub-bitmap from the given region of the parent bitmap that shares data with the parent bitmap.
		Bitmap::Bitmap(const Bitmap& other, const Rectangle& region) {

			// Create a sub-bitmap of the given bitmap. The new bitmap will share memory with the existing one.
			__bmp = al_create_sub_bitmap(other.AlPtr(), region.X(), region.Y(), region.Width(), region.Height());

			// It's fine to free sub-bitmaps.
			__free = true;

		}
		Bitmap::Bitmap(const Bitmap& other) {

			// Create a shallow copy of the source Bitmap. Both Bitmaps will share the same pixel data.
			__bmp = other.__bmp;

			// We will not free the underlying memory; this will be left up to the source Bitmap.
			__free = false;

		}
		Bitmap::Bitmap(Bitmap&& other) {

			__bmp = other.__bmp;
			__free = other.__free;

			other.__bmp = nullptr;
			other.__free = false;

		}
		Bitmap::~Bitmap() {

			if (__bmp && __free)
				Free();

		}

		Bitmap Bitmap::Clone() const {

			return Bitmap(al_clone_bitmap(__bmp), true);

		}
		Bitmap Bitmap::Clone(const Rectangle& region) const {

			// Create a new bitmap.
			Bitmap bmp(al_create_bitmap(region.Width(), region.Height()), true);

			// Copy the given bitmap onto the new bitmap.
			Drawing::Graphics(bmp).DrawBitmap(0, 0, *this, region);

			// Return the result.
			return bmp;

		}

		unsigned int Bitmap::Width() const {

			if (!__bmp)
				return 0U;

			return static_cast<unsigned int>(al_get_bitmap_width(__bmp));

		}
		unsigned int Bitmap::Height() const {

			if (!__bmp)
				return 0U;

			return static_cast<unsigned int>(al_get_bitmap_height(__bmp));

		}

		BitmapData Bitmap::Lock(IO::FileAccess access) {

			int flags;
			switch (access) {
			case IO::FileAccess::Read: flags = ALLEGRO_LOCK_READONLY; break;
			case IO::FileAccess::Write: flags = ALLEGRO_LOCK_WRITEONLY; break;
			case IO::FileAccess::ReadWrite: flags = ALLEGRO_LOCK_READWRITE; break;
			}

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(__bmp, al_get_bitmap_format(__bmp), flags);

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		BitmapData Bitmap::LockRegion(const Rectangle& region, IO::FileAccess access) {

			int flags;
			switch (access) {
			case IO::FileAccess::Read: flags = ALLEGRO_LOCK_READONLY; break;
			case IO::FileAccess::Write: flags = ALLEGRO_LOCK_WRITEONLY; break;
			case IO::FileAccess::ReadWrite: flags = ALLEGRO_LOCK_READWRITE; break;
			}

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap_region(__bmp, region.X(), region.Y(), region.Width(), region.Height(), al_get_bitmap_format(__bmp), flags);

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		void Bitmap::Unlock() {
			if (!IsLocked()) return;

			al_unlock_bitmap(__bmp);

		}
		bool Bitmap::IsLocked() const {

			return al_is_bitmap_locked(__bmp);

		}
		void Bitmap::SetPixel(int x, int y, const Color& color) {

			Drawing::Graphics(*this).DrawPoint(x, y, color);

		}
		Color Bitmap::GetPixel(int x, int y) const {

			ALLEGRO_COLOR px = al_get_pixel(__bmp, x, y);

			return Color::FromArgbf(px.r, px.g, px.b, px.a);

		}

		void Bitmap::ConvertMaskToAlpha(const Color& color) {

			ALLEGRO_COLOR al_color = al_map_rgba(color.R(), color.G(), color.B(), color.Alpha());

			al_convert_mask_to_alpha(__bmp, al_color);

		}

		ALLEGRO_BITMAP* Bitmap::AlPtr() const {

			return __bmp;

		}

		Bitmap& Bitmap::operator=(Bitmap& other) {

			ShallowCopy(other);
			return *this;

		}
		Bitmap& Bitmap::operator=(Bitmap&& other) {

			ShallowCopy(other);
			return *this;

		}
		Bitmap::operator bool() const {

			return (__bmp != nullptr);

		}

		BitmapData::BitmapData() {

			Scan0 = nullptr;
			PixelFormat = -1;
			Stride = 0;
			BytesPerPixel = 0;

		}

		void Bitmap::ShallowCopy(Bitmap& other) {

			// If we've already got data allocated, we need to free it before taking data from the other object.
			if (__free && __bmp)
				Free();

			// Perform a shallow copy of the other object.
			__bmp = other.__bmp;
			__free = other.__free;

			// Clear values from the other object.
			other.__bmp = nullptr;
			other.__free = false;

		}
		void Bitmap::Free() {

			al_destroy_bitmap(__bmp);
			__bmp = nullptr;
			__free = false;

		}

	}

}