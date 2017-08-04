#pragma once
#include "Framework.h"
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
			static ALLEGRO_FONT* FromFont(const Font& font);

		private:
			// It should not be possible to instantiate this class.
			AllegroAdapter() = default;

		};

	}
}