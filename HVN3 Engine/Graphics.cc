#include <stack>
#include "Graphics.h"
#include "Exception.h"
#include "Transform.h"
#include "Bitmap.h"

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

namespace Graphics {

	static Transform __global_transform;
	static std::stack<Rectangle> __clipping_region_stack;
	static std::stack<ALLEGRO_BITMAP*> __drawing_target_stack;

	void DrawRoundRect(const Rectangle& rect, float radius, const Color& color, float thickness) {

		// Note: 0.5 is added to the position of the rectangle to fix graphical glitches in Allegro (uneven corners).

		al_draw_rounded_rectangle(rect.X + 0.5f, rect.Y + 0.5f, rect.X2() + 0.5f, rect.Y2() + 0.5f, radius, radius, color.AlPtr(), thickness);

	}
	void DrawFilledRoundRect(const Rectangle& rect, float radius, const Color& color) {

		// Note: 0.5 is added to the position of the rectangle to fix graphical glitches in Allegro (uneven corners).

		al_draw_filled_rounded_rectangle(rect.X + 0.5f, rect.Y + 0.5f, rect.X2() + 0.5f, rect.Y2() + 0.5f, radius, radius, color.AlPtr());

	}

	void DrawRectangle(const Rectangle& rect, const Color& color, float thickness) {

		al_draw_rectangle(
			rect.X,
			rect.Y,
			rect.X2(),
			rect.Y2(),
			color.AlPtr(),
			thickness
		);

	}
	void DrawFilledRectangle(const Rectangle& rect, const Color& color) {

		al_draw_filled_rectangle(
			rect.X,
			rect.Y,
			rect.X2(),
			rect.Y2(),
			color.AlPtr()
		);

	}

	void DrawLine(const Line& line) {

		DrawLine(line, Color::Black, 1.0f);

	}
	void DrawLine(const Line& line, const Color& color, float thickness) {

		al_draw_line(line.First().X, line.First().Y, line.Second().X, line.Second().Y, color.AlPtr(), thickness);

	}

	void DrawCircle(float x, float y, float radius, const Color& color, float thickness) {

		al_draw_circle(
			x,
			y,
			radius,
			color.AlPtr(),
			thickness
		);

	}

	void DrawClear(const Color& color) {

		al_clear_to_color(color.AlPtr());

	}
	void DrawClear(Color* color) {

		al_clear_to_color(color->AlPtr());

	}

	void DrawText(float x, float y, const char* text, const Font* font, const Color& color, Alignment align) {

		int flags = 0;
		switch (align) {
		case Alignment::Left:
			flags |= ALLEGRO_ALIGN_LEFT;
			break;
		case Alignment::Center:
			flags |= ALLEGRO_ALIGN_CENTER;
			break;
		case Alignment::Right:
			flags |= ALLEGRO_ALIGN_RIGHT;
			break;
		}

		al_draw_text(font->AlPtr(), color.AlPtr(), x, y, flags, text);

	}
	void DrawText(float x, float y, const std::string& text, const Font* font, const Color& color) {

		DrawText(x, y, text.c_str(), font, color);

	}
	void DrawText(float x, float y, Utf8String& text, const Font* font, const Color& color) {

		DrawText(x, y, &text, font, color);

	}
	void DrawText(float x, float y, Utf8String* text, const Font* font, const Color& color) {

		al_draw_ustr(font->AlPtr(), color.AlPtr(), x, y, NULL, text->AlPtr());

	}

	void SetClippingRegion(int x, int y, int width, int height) {

		// Push the new clipping region onto the stack.
		__clipping_region_stack.push(Rectangle(x, y, width, height));

		// Apply the new clipping region.
		al_set_clipping_rectangle(x, y, width, height);

	}
	void SetClippingRegion(const Rectangle& rect) {

		SetClippingRegion(rect.X, rect.Y, rect.Width(), rect.Height());

	}
	Rectangle GetClippingRegion() {

		int x, y, w, h;
		al_get_clipping_rectangle(&x, &y, &w, &h);
		return Rectangle(x, y, w, h);

	}
	void ResetClippingRegion() {

		// Pop the current clipping region from the stack.
		__clipping_region_stack.pop();

		// If the stack is empty, reset the clipping region. Otherwise, use the next one.
		if (__clipping_region_stack.empty())
			al_reset_clipping_rectangle();
		else
			SetClippingRegion(__clipping_region_stack.top());


	}
	void SetDrawingTarget(const Bitmap& bitmap) {

		SetDrawingTarget(bitmap.AlPtr());

	}
	void SetDrawingTarget(ALLEGRO_BITMAP* target) {

		// Push the new drawing target onto the stack.
		__drawing_target_stack.push(target);

		// Set the new drawing target.
		al_set_target_bitmap(target);

	}
	Bitmap GetDrawingTarget() {

		// If the drawing target stack is empty, return a null Bitmap.
		if (__drawing_target_stack.empty())
			return Bitmap(nullptr, false);

		// Otherwise, return a Bitmap containing the target at the top of the stack.
		return Bitmap(__drawing_target_stack.top(), false);

	}

	void ResetDrawingTarget() {

		// If there is only one drawing target on the stack, don't allow it to be popped, because it acts as our default target.
		if (__drawing_target_stack.size() <= 1)
			return;

		// Pop the current drawing target from the stack.
		__drawing_target_stack.pop();

		// Set the drawing target to the target now at the top of the stack.
		al_set_target_bitmap(__drawing_target_stack.top());

	}
	void HoldBitmapDrawing(bool hold) {

		al_hold_bitmap_drawing(hold);

	}

	/*void DrawView(const View& view) {

		DrawView(view.Port().X, view.Port().Y, view);

	}
	void DrawView(float x, float y, const View& view) {

		// Get Bitmap to draw portion of.
		ALLEGRO_BITMAP* src_bitmap = Game::SceneSurface().AlPtr();

		// If the Bitmap is null, do nothing (no error).
		if (!src_bitmap) return;

		// Draw the View.
		Graphics::SetClippingRegion(x, y, view.Port().Width(), view.Port().Height());
		al_draw_scaled_rotated_bitmap(
			src_bitmap,
			view.Region().X + view.Region().Width() / 2.0f,
			view.Region().Y + view.Region().Height() / 2.0f,
			x + view.Port().Width() / 2.0f,
			x + view.Port().Height() / 2.0f,
			view.ScaleX(),
			view.ScaleY(),
			DegreesToRadians(view.Angle()),
			NULL
		);
		Graphics::ResetClippingRegion();

	}*/

	void DrawSprite(const Sprite& sprite, int subimage, float x, float y) {

		al_draw_bitmap(
			sprite[subimage % (int)sprite.Length()],
			x + sprite.Origin().X,
			y + sprite.Origin().Y,
			NULL
		);

	}
	void DrawSprite(const Sprite& sprite, int subimage, float x, float y, const Color& blend, float xscale, float yscale, float angle) {

		al_draw_tinted_scaled_rotated_bitmap(
			sprite[subimage % (int)sprite.Length()],
			al_map_rgba_f(blend.Rf() * blend.Alphaf(), blend.Gf() * blend.Alphaf(), blend.Bf() * blend.Alphaf(), blend.Alphaf()),
			sprite.Origin().X,
			sprite.Origin().Y,
			x,
			y,
			xscale,
			yscale,
			angle,
			NULL
		);

	}

	void DrawBitmap(const Bitmap& bitmap, float x, float y) {
		
		al_draw_bitmap(
			bitmap.__bmp,
			x,
			y,
			NULL
		);

	}
	void DrawBitmap(const Bitmap& bitmap, float x, float y, float xscale, float yscale) {

		al_draw_scaled_bitmap(
			bitmap.__bmp,
			0.0f,
			0.0f,
			bitmap.Width(),
			bitmap.Height(),
			x,
			y,
			bitmap.Width() * xscale,
			bitmap.Height() * yscale,
			NULL
		);

	}

	const Transform& GetTransform() {

		return __global_transform;

	}
	void SetTransform(const Transform& transform) {

		__global_transform = transform;
		al_use_transform(transform.AlPtr());

	}

}