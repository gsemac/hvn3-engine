#include "AllegroAdapter.h"
#include "Color.h"

namespace hvn3 {
	namespace System {

		ALLEGRO_COLOR AllegroAdapter::ToColor(const Color& color) {

			return al_map_rgba_f(color.Rf() * color.Alphaf(), color.Gf() * color.Alphaf(), color.Bf() * color.Alphaf(), color.Alphaf());

		}
		Color AllegroAdapter::FromColor(const ALLEGRO_COLOR& color) {

			return Color::FromArgbf(color.r, color.g, color.b, color.a);

		}
		ALLEGRO_FONT* AllegroAdapter::FromFont(const Font& font) {

			return font._font;

		}

	}
}