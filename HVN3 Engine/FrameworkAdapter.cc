#include "FrameworkAdapter.h"
#include "Color.h"

#ifdef HVN3_BACKEND_ALLEGRO5

namespace hvn3 {
	namespace System {

		ALLEGRO_COLOR FrameworkAdapter::ToColor(const Color& color) {

			return al_map_rgba_f(color.Rf() * color.Alphaf(), color.Gf() * color.Alphaf(), color.Bf() * color.Alphaf(), color.Alphaf());

		}
		Color FrameworkAdapter::FromColor(const ALLEGRO_COLOR& color) {

			return Color::FromArgbf(color.r, color.g, color.b, color.a);

		}
		ALLEGRO_FONT* FrameworkAdapter::FromFont(const Font& font) {

			return font._font;

		}

	}
}

#endif