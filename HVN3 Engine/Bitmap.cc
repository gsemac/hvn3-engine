#include "Bitmap.h"
#include "Graphics.h"

Bitmap::Bitmap(int width, int height) {

	__bmp = al_create_bitmap(width, height);
	__width = width;
	__height = height;
	__free = true;

}
Bitmap::Bitmap(const char* filename) {

	__bmp = al_load_bitmap(filename);
	__width = al_get_bitmap_width(__bmp);
	__height = al_get_bitmap_height(__bmp);
	__free = true;

}
Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap, bool managed) {

	__bmp = bitmap;
	__width = 0;
	__height = 0;
	__free = managed;

	if (bitmap) {
		__width = al_get_bitmap_width(__bmp);
		__height = al_get_bitmap_height(__bmp);
	} 

}
Bitmap::Bitmap(ALLEGRO_BITMAP* bitmap, const Rectangle& region) {

	__bmp = al_create_bitmap(region.Width(), region.Height());
	Graphics::SetDrawingTarget(__bmp);
	al_draw_bitmap_region(bitmap, region.X, region.Y, region.Width(), region.Height(), 0, 0, NULL);
	Graphics::ResetDrawingTarget();
	__free = true;

}
Bitmap::Bitmap(const Bitmap& other) {

	__bmp = al_clone_bitmap(other.__bmp);
	__width = al_get_bitmap_width(__bmp);
	__height = al_get_bitmap_height(__bmp);
	__free = true;

}
Bitmap::Bitmap(const Bitmap& other, const Rectangle& region) : Bitmap(other.AlPtr(), region) {

}
Bitmap::Bitmap(Bitmap&& other) {

	__bmp = other.__bmp;
	__width = other.__width;
	__height = other.__height;
	__free = other.__free;

	other.__bmp = nullptr;
	other.__width = 0;
	other.__height = 0;

}
Bitmap::~Bitmap() {

	if (__bmp && __free)
		al_destroy_bitmap(__bmp);

}

Bitmap Bitmap::RefBitmap(const Bitmap& other, const Rectangle& region) {

	return RefBitmap(other.AlPtr(), region);

}
Bitmap Bitmap::RefBitmap(ALLEGRO_BITMAP* other, const Rectangle& region) {

	Bitmap bm(al_create_sub_bitmap(other, region.X, region.Y, region.Width(), region.Height()));

	return bm;

}
bool Bitmap::IsRefBitmap() const {

	return al_is_sub_bitmap(__bmp);

}

unsigned int Bitmap::Width() const {

	return __width;

}
unsigned int Bitmap::Height() const {

	return __height;

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

	ALLEGRO_LOCKED_REGION* lr = al_lock_bitmap_region(__bmp, region.X, region.Y, region.Width(), region.Height(), al_get_bitmap_format(__bmp), flags);

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

	Graphics::SetDrawingTarget(__bmp);
	al_put_pixel(x, y, color.AlPtr());
	Graphics::ResetDrawingTarget();

}
Color Bitmap::GetPixel(int x, int y) const {

	return Color(al_get_pixel(__bmp, x, y));
	
}

ALLEGRO_BITMAP* Bitmap::AlPtr() const {

	return __bmp;

}

Bitmap& Bitmap::operator=(Bitmap&& other) {

	__bmp = other.__bmp;
	__width = other.__width;
	__height = other.__height;

	other.__bmp = nullptr;
	other.__width = 0;
	other.__height = 0;

	return *this;

}

BitmapData::BitmapData() {

	Scan0 = nullptr;
	PixelFormat = -1;
	Stride = 0;
	BytesPerPixel = 0;

}