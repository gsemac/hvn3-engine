#include "hvn3/graphics/Bitmap.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/allegro/AllegroAdapter.h"
#include <allegro5/allegro_memfile.h>


namespace hvn3 {

	namespace Graphics {

		Bitmap::Bitmap() {

			_bitmap = nullptr;
			_free = false;

		}
		Bitmap::Bitmap(int width, int height) :
			Bitmap() {

			// If either the width or height is 0, do not allocate any memory for the bitmap.
			if (width == 0 || height == 0) {
				_bitmap = nullptr;
				_free = false;
				return;
			}

			// Otherwise, allocate memory for the bitmap.
			_bitmap = al_create_bitmap(width, height);
			_free = true;

		}
		Bitmap::Bitmap(const char* filename) :
			Bitmap() {

			_bitmap = al_load_bitmap(filename);
			_free = true;

		}
		Bitmap::Bitmap(const char* filename, const Color& alpha) :
			Bitmap(filename) {

			ConvertMaskToAlpha(alpha);

		}
		Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap, bool free) :
			Bitmap() {

			_bitmap = bitmap;
			_free = free;

		}
		Bitmap::Bitmap(uint8_t* buffer, size_t buffer_size, Imaging::ImageFormat format) :
			Bitmap() {

			if (buffer == nullptr || buffer_size <= 0)
				return;

			ALLEGRO_FILE* file = al_open_memfile(buffer, buffer_size, "r");

			_bitmap = al_load_bitmap_f(file, ImageFormatToFileExtension(format).c_str());
			_free = true;

			al_fclose(file);

		}
		// Creates a sub-bitmap from the given region of the parent bitmap that shares data with the parent bitmap.
		Bitmap::Bitmap(const Bitmap& other, const RectangleI& region) :
			Bitmap() {

			// Create a sub-bitmap of the given bitmap. The new bitmap will share memory with the existing one.
			_bitmap = al_create_sub_bitmap(other.AlPtr(), region.X(), region.Y(), region.Width(), region.Height());

			// Sub-bitmaps still need to be freed (this will not affect the parent bitmap).
			_free = true;

		}
		Bitmap::Bitmap(const Bitmap& other) :
			Bitmap() {

			// Create a shallow copy of the source Bitmap. Both Bitmaps will share the same pixel data.
			_shallowCopy(other);

		}
		Bitmap::Bitmap(Bitmap&& other) :
			Bitmap() {

			_moveCopy(other);

		}
		Bitmap::~Bitmap() {

			if (_bitmap != nullptr && _free)
				_freeBitmap();

		}

		Bitmap Bitmap::Clone() const {

			return Bitmap(al_clone_bitmap(_bitmap), true);

		}
		Bitmap Bitmap::Clone(const RectangleF& region) const {

			// Create a new bitmap.
			Bitmap bmp(al_create_bitmap(region.Width(), region.Height()), true);

			// Copy the given bitmap onto the new bitmap.
			Graphics::Graphics(bmp).DrawBitmap(0, 0, *this, region);

			// Return the result.
			return bmp;

		}

		unsigned int Bitmap::Width() const {

			if (_bitmap == nullptr)
				return 0U;

			return static_cast<unsigned int>(al_get_bitmap_width(_bitmap));

		}
		unsigned int Bitmap::Height() const {

			if (_bitmap == nullptr)
				return 0U;

			return static_cast<unsigned int>(al_get_bitmap_height(_bitmap));

		}

		BitmapData Bitmap::Lock(IO::FileAccess access) {

			int flags;
			switch (access) {
			case IO::FileAccess::Read: flags = ALLEGRO_LOCK_READONLY; break;
			case IO::FileAccess::Write: flags = ALLEGRO_LOCK_WRITEONLY; break;
			case IO::FileAccess::ReadWrite: flags = ALLEGRO_LOCK_READWRITE; break;
			}

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap(_bitmap, al_get_bitmap_format(_bitmap), flags);

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

			ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap_region(_bitmap, region.X(), region.Y(), region.Width(), region.Height(), al_get_bitmap_format(_bitmap), flags);

			BitmapData bmpdata;
			bmpdata.Scan0 = (unsigned char*)lr->data;
			bmpdata.PixelFormat = lr->format;
			bmpdata.Stride = lr->pitch;
			bmpdata.BytesPerPixel = lr->pixel_size;

			return bmpdata;

		}
		void Bitmap::Unlock() {
			if (!IsLocked()) return;

			al_unlock_bitmap(_bitmap);

		}
		bool Bitmap::IsLocked() const {

			return al_is_bitmap_locked(_bitmap);

		}
		void Bitmap::SetPixel(int x, int y, const Color& color) {

			Graphics::Graphics(*this).DrawPoint(x, y, color);

		}
		Color Bitmap::GetPixel(int x, int y) const {

			ALLEGRO_COLOR px = al_get_pixel(_bitmap, x, y);

			return Color::FromArgbf(px.r, px.g, px.b, px.a);

		}

		void Bitmap::ConvertMaskToAlpha(const Color& color) {

			ALLEGRO_COLOR al_color = al_map_rgba(color.R(), color.G(), color.B(), color.Alpha());

			al_convert_mask_to_alpha(_bitmap, al_color);

		}

		bool Bitmap::IsVideoBitmap() const {

			return al_get_bitmap_flags(_bitmap) & ALLEGRO_VIDEO_BITMAP;

		}
		void Bitmap::ConvertToVideoBitmap() {

			al_convert_bitmap(_bitmap);

		}

		ALLEGRO_BITMAP* Bitmap::AlPtr() const {

			return _bitmap;

		}

		Bitmap& Bitmap::operator=(Bitmap& other) {

			_shallowCopy(other);

			return *this;

		}
		Bitmap& Bitmap::operator=(Bitmap&& other) {

			_moveCopy(other);

			return *this;

		}
		Bitmap::operator bool() const {

			return (_bitmap != nullptr);

		}


		BitmapFlags Bitmap::NewBitmapFlags() {

			return System::AllegroAdapter::FromBitmapFlags(al_get_new_bitmap_flags());

		}
		void Bitmap::SetNewBitmapFlags(BitmapFlags flags) {

			al_set_new_bitmap_flags(System::AllegroAdapter::ToBitmapFlags(flags));

		}
		void Bitmap::ConvertMemoryBitmapsToVideoBitmaps() {

			al_convert_memory_bitmaps();

		}



		BitmapData::BitmapData() {

			Scan0 = nullptr;
			PixelFormat = -1;
			Stride = 0;
			BytesPerPixel = 0;

		}



		void Bitmap::_shallowCopy(const Bitmap& other) {

			// Free any bitmap data we may have previously created.
			if (_free && _bitmap != nullptr)
				_freeBitmap();

			_bitmap = other._bitmap;

			// Do not free, since the original bitmap owns the bitmap data.
			_free = false;

		}
		void Bitmap::_moveCopy(Bitmap& other) {

			_shallowCopy(other);

			// Set free to true, since we have taken ownership of the bitmap data.
			_free = other._free;

			other._bitmap = nullptr;
			other._free = false;

		}
		void Bitmap::_freeBitmap() {

			al_destroy_bitmap(_bitmap);

			_bitmap = nullptr;
			_free = false;

		}

	}

}