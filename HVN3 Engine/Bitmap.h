#ifndef __BITMAP_H
#define __BITMAP_H
#include <allegro5\allegro.h>
#include "ISizeable.h"
#include "Geometry.h"
#include "Color.h"
#include "IO_File.h"

namespace Drawing {

	struct BitmapData {
		BitmapData();
		unsigned char* Scan0;
		int PixelFormat;
		int Stride;
		int BytesPerPixel;
	};
	
	class Bitmap {

	public:
		Bitmap(int width, int height);
		Bitmap(const char* filename);
		Bitmap(ALLEGRO_BITMAP* bitmap, bool free = true);
		Bitmap(const Bitmap& other, const Rectangle& region);
		Bitmap(const Bitmap& other);
		Bitmap(Bitmap&& other);
		~Bitmap();

		Bitmap Clone() const;
		Bitmap Clone(const Rectangle& region) const;

		//static Bitmap RefBitmap(const Bitmap& other, const Rectangle& region);
		//static Bitmap RefBitmap(ALLEGRO_BITMAP* other, const Rectangle& region);
		//bool IsRefBitmap() const;

		unsigned int Width() const;
		unsigned int Height() const;

		BitmapData Lock(IO::FileAccess access = IO::FileAccess::ReadWrite);
		BitmapData LockRegion(const Rectangle& region, IO::FileAccess access = IO::FileAccess::ReadWrite);
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
		Bitmap();
		ALLEGRO_BITMAP* __bmp;
		bool __free;

	};

}

#endif