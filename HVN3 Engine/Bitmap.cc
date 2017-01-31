#include "Bitmap.h"
#include "Graphics.h"

namespace Drawing {

	Bitmap::Bitmap() {

		__bmp = nullptr;
		__free = false;

	}
	Bitmap::Bitmap(int width, int height) {

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
	//Bitmap::Bitmap(const Bitmap& other, const Rectangle& region) {
	//
	//	// Create a new bitmap.
	//	__bmp = al_create_bitmap(region.Width(), region.Height());
	//
	//	// Copy the given bitmap onto the new bitmap.
	//	Drawing::Graphics(Bitmap(__bmp, false)).DrawBitmap(other, region, 0.0f, 0.0f);
	//
	//	// Set free to true, because we need to free the new bitmap.
	//	__free = true;
	//
	//}
	//Bitmap::Bitmap(const Bitmap& other) {
	//
	//	__bmp = al_clone_bitmap(other.__bmp);
	//	__width = al_get_bitmap_width(__bmp);
	//	__height = al_get_bitmap_height(__bmp);
	//	__free = true;
	//
	//}
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
			al_destroy_bitmap(__bmp);

	}

	Bitmap Bitmap::Clone() {

		return Bitmap(al_clone_bitmap(__bmp), true);

	}

	//Bitmap Bitmap::RefBitmap(const Bitmap& other, const Rectangle& region) {
	//
	//	return RefBitmap(other.AlPtr(), region);
	//
	//}
	//Bitmap Bitmap::RefBitmap(ALLEGRO_BITMAP* other, const Rectangle& region) {
	//
	//	Bitmap bm(al_create_sub_bitmap(other, region.X(), region.Y(), region.Width(), region.Height()));
	//
	//	return bm;
	//
	//}
	//bool Bitmap::IsRefBitmap() const {
	//
	//	return al_is_sub_bitmap(__bmp);
	//
	//}

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

		return Color(al_get_pixel(__bmp, x, y));

	}

	ALLEGRO_BITMAP* Bitmap::AlPtr() const {

		return __bmp;

	}

	Bitmap& Bitmap::operator=(Bitmap&& other) {

		// Perform a shallow copy of the other object.
		__bmp = other.__bmp;
		__free = other.__free;

		// Clear values from the other object.
		other.__bmp = nullptr;
		other.__free = false;

		return *this;

	}
	Bitmap::operator bool() const {

		return __bmp;

	}

	BitmapData::BitmapData() {

		Scan0 = nullptr;
		PixelFormat = -1;
		Stride = 0;
		BytesPerPixel = 0;

	}

}