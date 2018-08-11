#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/graphics/BitmapUtils.h"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>

namespace hvn3 {
	namespace Graphics {

		void ConvertMaskToAlpha(Bitmap& bitmap, const Color& color) {
			al_convert_mask_to_alpha(System::AllegroAdapter::ToBitmap(bitmap), System::AllegroAdapter::ToColor(color));
		}
		void SetAlphaFromBitmap(Bitmap& dest, Bitmap& src) {

			// Lock both the destination and source bitmaps. Keep in mind that they might refer to the same bitmap.
			// By checking if the source is locked after locking the destination, we can make sure we don't lock the same bitmap twice.

			bool dest_already_locked = dest.IsLocked();
			if (!dest_already_locked)
				dest.Lock();

			bool src_already_locked = src.IsLocked();
			if (!src_already_locked)
				src.Lock();

			// The value of the source bitmap is multiplied with the alpha of the destination bitmap.
			// Ideally, the source bitmap is greyscale. White corresponds to opaque, and black corresponds to transparent.

			for (int j = 0; j < Math::Min(dest.Height(), src.Height()); ++j)
				for (int i = 0; i < Math::Min(dest.Width(), src.Width()); ++i) {

					Color dest_color = dest.GetPixel(i, j);
					Color src_color = src.GetPixel(i, j);

					float dest_alpha = dest_color.Alphaf();
					float src_value = src_color.Brightness();

					Color new_color = Color::FromArgbf(dest_color.Rf(), dest_color.Gf(), dest_color.Bf(), src_value * dest_alpha);

					dest.SetPixel(i, j, new_color);

				}

			if (!dest_already_locked)
				dest.Unlock();

			if (!src_already_locked && src.IsLocked())
				src.Unlock();

		}
		void ConvertToGreyscale(Bitmap& bitmap) {

			bool already_locked = bitmap.IsLocked();
			if (!already_locked)
				bitmap.Lock();

			for (int j = 0; j < bitmap.Height(); ++j)
				for (int i = 0; i < bitmap.Width(); ++i) {

					Color old_color = bitmap.GetPixel(i, j);
					unsigned char greyscale = static_cast<unsigned char>(old_color.R() * 0.299f + old_color.G() * 0.587f + old_color.B() * 0.114f);

					bitmap.SetPixel(i, j, Color::FromArgb(greyscale, greyscale, greyscale, old_color.Alpha()));

				}

			if (!already_locked)
				bitmap.Unlock();

		}

	}
}