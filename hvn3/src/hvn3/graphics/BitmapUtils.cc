#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/graphics/BitmapUtils.h"
#include <allegro5/allegro_image.h>

namespace hvn3 {
	namespace Graphics {

		void ConvertMaskToAlpha(Bitmap& bitmap, const Color& color) {
			al_convert_mask_to_alpha(System::AllegroAdapter::ToBitmap(bitmap), System::AllegroAdapter::ToColor(color));
		}
		void SetAlphaFromBitmap(Bitmap& dest, Bitmap& src) {

			dest.Lock();

			// Keep in mind that the destination and source bitmaps could be the same bitmap.
			if (dest != src)
				src.Lock();

			// The value of the source bitmap is multiplied with the alpha of the destination bitmap.
			// Ideally, the source bitmap is greyscale. White corresponds to opaque, and black corresponds to transparent.

			for (int i = 0; i < Math::Min(dest.Height(), src.Height()); ++i)
				for (int j = 0; j < Math::Min(dest.Width(), src.Width()); ++j) {

					Color dest_color = dest.GetPixel(i, j);
					Color src_color = src.GetPixel(i, j);

					float dest_alpha = dest_color.Alphaf();
					float src_value = src_color.Brightness();

					Color new_color = Color::FromArgbf(dest_color.Rf(), dest_color.Gf(), dest_color.Bf(), src_value * dest_alpha);

					dest.SetPixel(i, j, new_color);

				}

			dest.Unlock();

			if (dest != src)
				src.Unlock();

		}
		void ConvertToGreyscale(Bitmap& bitmap) {

			bitmap.Lock();

			for (int i = 0; i < bitmap.Height(); ++i)
				for (int j = 0; j < bitmap.Width(); ++j) {

					Color old_color = bitmap.GetPixel(i, j);
					unsigned char greyscale = static_cast<unsigned char>(old_color.R() * 0.299f + old_color.G() * 0.587f + old_color.B() * 0.114f);

					bitmap.SetPixel(i, j, Color::FromArgb(greyscale, greyscale, greyscale, old_color.Alpha()));

				}

			bitmap.Unlock();

		}

	}
}