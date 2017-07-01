#include "Bitmap.h"
#include "Graphics.h"
#include "FrameworkAdapter.h"
#define ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS 4096

namespace hvn3 {

	namespace Drawing {

		Bitmap::Bitmap() {

			_bmp = nullptr;
			_free = false;

		}
		Bitmap::Bitmap(int width, int height) {

			// If either the width or height is 0, do not allocate any memory for the bitmap.
			if (width == 0 || height == 0) {
				_bmp = nullptr;
				_free = false;
				return;
			}

			// Otherwise, allocate memory for the bitmap.
			_bmp = al_create_bitmap(width, height);
			_free = true;

		}
		Bitmap::Bitmap(const char* filename) {

			_bmp = al_load_bitmap(filename);
			_free = true;

		}
		Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap, bool free) {

			_bmp = bitmap;
			_free = free;

		}
		// Creates a sub-bitmap from the given region of the parent bitmap that shares data with the parent bitmap.
		Bitmap::Bitmap(const Bitmap& other, const RectangleI& region) {

			// Create a sub-bitmap of the given bitmap. The new bitmap will share memory with the existing one.
			_bmp = al_create_sub_bitmap(other.AlPtr(), region.X(), region.Y(), region.Width(), region.Height());

			// It's fine to free sub-bitmaps.
			_free = true;

		}
		Bitmap::Bitmap(const Bitmap& other) {

			// Create a shallow copy of the source Bitmap. Both Bitmaps will share the same pixel data.
			_bmp = other._bmp;

			// We will not free the underlying memory; this will be left up to the source Bitmap.
			_free = false;

		}
		Bitmap::Bitmap(Bitmap&& other) {

			_bmp = other._bmp;
			_free = other._free;

			other._bmp = nullptr;
			other._free = false;

		}
		Bitmap::~Bitmap() {

			if (_bmp && _free)
				Free();

		}

		Bitmap Bitmap::Clone() const {

			return Bitmap(al_clone_bitmap(_bmp), true);

		}
		Bitmap Bitmap::Clone(const RectangleF& region) const {

			// Create a new bitmap.
			Bitmap bmp(al_create_bitmap(region.Width(), region.Height()), true);

			// Copy the given bitmap onto the new bitmap.
			Drawing::Graphics(bmp).DrawBitmap(0, 0, this, region);

			// Return the result.
			return bmp;

		}

		unsigned int Bitmap::Width() const {

			if (!_bmp)
				return 0U;

			return static_cast<unsigned int>(al_get_bitmap_width(_bmp));

		}
		unsigned int Bitmap::Height() const {

			if (!_bmp)
				return 0U;

			return static_cast<unsigned int>(al_get_bitmap_height(_bmp));

		}

		BitmapData Bitmap::Lock(IO::FileAccess access) {

			int flags;
			switch (access) {
			case IO::FileAccess::Read: flags = ALLEGRO_LOCK_READONLY; break;
			case IO::FileAccess::Write: flags = ALLEGRO_LOCK_WRITEONLY; break;
			case IO::FileAccess::ReadWrite: flags = ALLEGRO_LOCK_READWRITE; break;
			}

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(_bmp, al_get_bitmap_format(_bmp), flags);

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		BitmapData Bitmap::LockRegion(const RectangleI& region, IO::FileAccess access) {

			int flags;
			switch (access) {
			case IO::FileAccess::Read: flags = ALLEGRO_LOCK_READONLY; break;
			case IO::FileAccess::Write: flags = ALLEGRO_LOCK_WRITEONLY; break;
			case IO::FileAccess::ReadWrite: flags = ALLEGRO_LOCK_READWRITE; break;
			}

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap_region(_bmp, region.X(), region.Y(), region.Width(), region.Height(), al_get_bitmap_format(_bmp), flags);

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		void Bitmap::Unlock() {
			if (!IsLocked()) return;

			al_unlock_bitmap(_bmp);

		}
		bool Bitmap::IsLocked() const {

			return al_is_bitmap_locked(_bmp);

		}
		void Bitmap::SetPixel(int x, int y, const Color& color) {

			Drawing::Graphics(*this).DrawPoint(x, y, color);

		}
		Color Bitmap::GetPixel(int x, int y) const {

			ALLEGRO_COLOR px = al_get_pixel(_bmp, x, y);

			return Color::FromArgbf(px.r, px.g, px.b, px.a);

		}

		void Bitmap::ConvertMaskToAlpha(const Color& color) {

			ALLEGRO_COLOR al_color = al_map_rgba(color.R(), color.G(), color.B(), color.Alpha());

			al_convert_mask_to_alpha(_bmp, al_color);

		}

		ALLEGRO_BITMAP* Bitmap::AlPtr() const {

			return _bmp;

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

			return (_bmp != nullptr);

		}

		BitmapData::BitmapData() {

			Scan0 = nullptr;
			PixelFormat = -1;
			Stride = 0;
			BytesPerPixel = 0;

		}

		void Bitmap::ShallowCopy(Bitmap& other) {

			// If we've already got data allocated, we need to free it before taking data from the other object.
			if (_free && _bmp)
				Free();

			// Perform a shallow copy of the other object.
			_bmp = other._bmp;
			_free = other._free;

			// Clear values from the other object.
			other._bmp = nullptr;
			other._free = false;

		}
		void Bitmap::Free() {

			al_destroy_bitmap(_bmp);
			_bmp = nullptr;
			_free = false;

		}

		// Static helper functions
		int ConvertBitmapFlags(BitmapFlags flags) {

			int al_flags = 0;

			if (HasFlag(flags, BitmapFlags::Default))
				al_flags |= ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS;

			if (HasFlag(flags, BitmapFlags::MinLinear))
				al_flags |= ALLEGRO_MIN_LINEAR;
	
			if (HasFlag(flags, BitmapFlags::MagLinear))
				al_flags |= ALLEGRO_MAG_LINEAR;

			return al_flags;

		}

		// Free functions
		BitmapFlags NewBitmapFlags() {

			int al_flags = al_get_new_bitmap_flags();
			int my_flags = 0;

			if (al_flags & ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS)
				my_flags |= ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS;

			if (al_flags & ALLEGRO_MIN_LINEAR)
				my_flags |= static_cast<int>(BitmapFlags::MinLinear);

			if (al_flags & ALLEGRO_MAG_LINEAR)
				my_flags |= static_cast<int>(BitmapFlags::MagLinear);

			return (BitmapFlags)my_flags;

		}
		void SetNewBitmapFlags(BitmapFlags flags) {

			al_set_new_bitmap_flags(ConvertBitmapFlags(flags));

		}

	}

}