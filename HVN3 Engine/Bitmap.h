#ifndef __BITMAP_H
#define __BITMAP_H
#include <allegro5\allegro.h>
#include "ISizeable.h"
#include "Geometry.h"
#include "Color.h"
#include "IO_File.h"
#include "Graphics.h"

namespace Graphics {
	void DrawBitmap(const Bitmap& bitmap, float x, float y);
	void DrawBitmap(const Bitmap& bitmap, float x, float y, float xscale, float yscale);
}

struct BitmapData {
	BitmapData();
	unsigned char* Scan0;
	int PixelFormat;
	int Stride;
	int BytesPerPixel;
};

class Bitmap {
	friend void Graphics::DrawBitmap(const Bitmap& bitmap, float x, float y);
	friend void Graphics::DrawBitmap(const Bitmap& bitmap, float x, float y, float xscale, float yscale);

private:
	ALLEGRO_BITMAP* __bmp;
	unsigned int __width, __height;
	bool __free;

public:
	Bitmap(int width, int height);
	Bitmap(const char* filename);
	Bitmap(ALLEGRO_BITMAP* bitmap, bool free = true);
	Bitmap(ALLEGRO_BITMAP* bitmap, const Rectangle& region);
	Bitmap(const Bitmap& other);
	Bitmap(const Bitmap& other, const Rectangle& region);
	Bitmap(Bitmap&& other);
	~Bitmap();

	static Bitmap RefBitmap(const Bitmap& other, const Rectangle& region);
	static Bitmap RefBitmap(ALLEGRO_BITMAP* other, const Rectangle& region);
	bool IsRefBitmap() const;

	unsigned int Width() const;
	unsigned int Height() const;

	BitmapData Lock(IO::FileAccess access = IO::FileAccess::ReadWrite);
	BitmapData LockRegion(const Rectangle& region, IO::FileAccess access = IO::FileAccess::ReadWrite);
	void Unlock();
	bool IsLocked() const;

	void SetPixel(int x, int y, const Color& color);
	Color GetPixel(int x, int y) const;

	ALLEGRO_BITMAP* AlPtr() const;

	Bitmap& operator=(Bitmap&& other);

};

#endif