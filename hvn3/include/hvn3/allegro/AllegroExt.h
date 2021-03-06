#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

struct ALLEGRO_BITMAP;

namespace hvn3 {

	void al_draw_wrapped_text(const ALLEGRO_FONT*, ALLEGRO_COLOR, float, float, float, float, int, char const*);
	void al_draw_shadow_ustr(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, ALLEGRO_COLOR shadow_color, float x, float y, int flags, const ALLEGRO_USTR* ustr);
	void al_draw_shadow_text(const ALLEGRO_FONT*, ALLEGRO_COLOR, ALLEGRO_COLOR, float, float, int, const char*);
	void al_draw_gradient_rectangle(float x, float y, float width, float height, ALLEGRO_COLOR color_top, ALLEGRO_COLOR color_bottom);
	void al_draw_horizontal_gradient_rectangle(float x, float y, float width, float height, ALLEGRO_COLOR color_left, ALLEGRO_COLOR color_right);

	void al_draw_line(float x1, float y1, float x2, float y2, ALLEGRO_COLOR start_color, ALLEGRO_COLOR end_color, float thickness);

	ALLEGRO_BITMAP* al_clone_sub_bitmap(ALLEGRO_BITMAP* bitmap);
		
}