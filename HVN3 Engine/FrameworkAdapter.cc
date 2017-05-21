#include "FrameworkAdapter.h"

#ifdef HVN3_BACKEND_ALLEGRO5

ALLEGRO_COLOR FrameworkAdapter::ToColor(const Color& color) {

	return al_map_rgba(color.R(), color.G(), color.B(), color.Alpha());

}
Color FrameworkAdapter::FromColor(const ALLEGRO_COLOR& color) {

	return Color::FromArgbf(color.r, color.g, color.b, color.a);

}

#endif