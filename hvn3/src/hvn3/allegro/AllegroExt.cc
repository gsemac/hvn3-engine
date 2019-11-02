#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/exceptions/Exception.h"
#include <allegro5/allegro_image.h>

namespace hvn3 {

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

	void al_draw_line(float x1, float y1, float x2, float y2, ALLEGRO_COLOR start_color, ALLEGRO_COLOR end_color, float thickness) {
		
		// This is the implementation for al_draw_line, modified to have different colors for the start and end points.

		// Original implementation:
		//	https://github.com/liballeg/allegro5/blob/d7757184d335d400460808eff8e0d19c9f557673/addons/primitives/high_primitives.c#L77

		if (thickness > 0) {

			int ii;
			float tx, ty;
			float len = std::hypotf(x2 - x1, y2 - y1);

			ALLEGRO_VERTEX vtx[4];

			if (len == 0)
				return;

			tx = 0.5f * thickness * (y2 - y1) / len;
			ty = 0.5f * thickness * -(x2 - x1) / len;

			vtx[0].x = x1 + tx; vtx[0].y = y1 + ty;
			vtx[1].x = x1 - tx; vtx[1].y = y1 - ty;
			vtx[2].x = x2 - tx; vtx[2].y = y2 - ty;
			vtx[3].x = x2 + tx; vtx[3].y = y2 + ty;

			for (ii = 0; ii < 2; ii++) {
				vtx[ii].color = start_color;
				vtx[ii].z = 0;
			}
			for (ii = 2; ii < 4; ii++) {
				vtx[ii].color = end_color;
				vtx[ii].z = 0;
			}

			al_draw_prim(vtx, 0, 0, 0, 4, ALLEGRO_PRIM_TRIANGLE_FAN);

		}
		else {
			ALLEGRO_VERTEX vtx[2];

			vtx[0].x = x1; vtx[0].y = y1;
			vtx[1].x = x2; vtx[1].y = y2;

			vtx[0].color = start_color;
			vtx[1].color = end_color;
			vtx[0].z = 0;
			vtx[1].z = 0;

			al_draw_prim(vtx, 0, 0, 0, 2, ALLEGRO_PRIM_LINE_LIST);
		}

	}

	ALLEGRO_BITMAP* al_clone_sub_bitmap(ALLEGRO_BITMAP* bitmap) {

		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);

		return al_create_sub_bitmap(bitmap, 0, 0, w, h);

	}

}