#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/graphics/Display.h"
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include <allegro5/allegro.h>

namespace hvn3 {

	class Color;
	class Font;

	namespace System {

		class AllegroAdapter {

		public:
			// Creates an ALLEGRO_COLOR from a Color instance.
			static ALLEGRO_COLOR ToColor(const Color& color);
			// Creates a Color from an ALLEGRO_COLOR instance.
			static Color FromColor(const ALLEGRO_COLOR& color);
			// Returns a pointer to the underlying ALLEGRO_FONT from a font instance.
			static ALLEGRO_FONT* ToFont(const Font& font);
			// Returns a pointer to the underlying ALLEGRO_BITMAP from a bitmap instance.
			static ALLEGRO_BITMAP* ToBitmap(const Graphics::Bitmap& bitmap);
			// Converts display flags to Allegro display flags.
			static int ToDisplayFlags(DisplayFlags flags);

		private:
			// It should not be possible to instantiate this class.
			AllegroAdapter() = default;

		};

	}
}