#include "AllegroExtensions.h"
#include "Exception.h"

void al_draw_wrapped_text(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, float w, float h, int flags, char const *text) {

	throw NotImplementedException();

}
void al_draw_shadow_ustr(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, ALLEGRO_COLOR shadow_color, float x, float y, int flags, const ALLEGRO_USTR* ustr) {

	al_draw_ustr(font, shadow_color, x + 1, y + 1, flags, ustr);
	al_draw_ustr(font, color, x, y, flags, ustr);

}
void al_draw_shadow_text(const ALLEGRO_FONT* font, ALLEGRO_COLOR color, ALLEGRO_COLOR shadow_color, float x, float y, int flags, const char* str) {

	al_draw_text(font, shadow_color, x + 1, y + 1, flags, str);
	al_draw_text(font, color, x, y, flags, str);

}
void al_draw_gradient_rectangle(float x, float y, float width, float height, ALLEGRO_COLOR color_top, ALLEGRO_COLOR color_bottom) {


	ALLEGRO_VERTEX v[] = {
		{ x, y, 0, 0, 0, color_top },
		{ x + width, y, 0, 0, 0, color_top },
		{ x, y + height, 0, 0, 0, color_bottom },
		{ x + width, y + height, 0, 0, 0, color_bottom }
	};
	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);

}
void al_draw_horizontal_gradient_rectangle(float x, float y, float width, float height, ALLEGRO_COLOR color_left, ALLEGRO_COLOR color_right) {

	ALLEGRO_VERTEX v[] = {
		{ x, y, 0, 0, 0, color_left },
		{ x + width, y, 0, 0, 0, color_right },
		{ x, y + height, 0, 0, 0, color_left },
		{ x + width, y + height, 0, 0, 0, color_right }
	};
	al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP);

}