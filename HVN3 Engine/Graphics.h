#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string>
#include "Color.h"
#include "Geometry.h"
#include "Font.h"
#include "UTF8String.h"
//#include "Bitmap.h"
#include "View.h"
#include "Transform.h"

class Bitmap;

void al_draw_wrapped_text(const ALLEGRO_FONT*, ALLEGRO_COLOR, float, float, float, float, int, char const*);
void al_draw_shadow_ustr(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, ALLEGRO_COLOR shadow_color, float x, float y, int flags, const ALLEGRO_USTR* ustr);
void al_draw_shadow_text(const ALLEGRO_FONT*, ALLEGRO_COLOR, ALLEGRO_COLOR, float, float, int, const char*);
void al_draw_gradient_rectangle(float x, float y, float width, float height, ALLEGRO_COLOR color_top, ALLEGRO_COLOR color_bottom);
void al_draw_horizontal_gradient_rectangle(float x, float y, float width, float height, ALLEGRO_COLOR color_left, ALLEGRO_COLOR color_right);

namespace Graphics {

	void DrawRoundRect(const Rectangle& rect, float radius, const Color& color, float thickness);
	void DrawFilledRoundRect(const Rectangle& rect, float radius, const Color& color);
	void DrawRectangle(const Rectangle& rect, const Color& color, float thickness);
	void DrawFilledRectangle(const Rectangle& rect, const Color& color);
	void DrawLine(const Line& line);
	void DrawLine(const Line& line, const Color& color, float thickness);
	void DrawCircle(float x, float y, float radius, const Color& color, float thickness);
	void DrawClear(const Color& color);
	void DrawClear(Color* color);
	void DrawText(float x, float y, const char* text, const Font* font, const Color& color, Alignment align = Alignment::Left);
	void DrawText(float x, float y, const std::string& text, const Font* font, const Color& color);
	void DrawText(float x, float y, Utf8String& text, const Font* font, const Color& color);
	void DrawText(float x, float y, Utf8String* text, const Font* font, const Color& color);
	void SetClippingRegion(int x, int y, int width, int height);
	void SetClippingRegion(const Rectangle& rect);
	Rectangle GetClippingRegion();
	void ResetClippingRegion();
	void SetDrawingTarget(const Bitmap& bitmap);
	void SetDrawingTarget(ALLEGRO_BITMAP* target);
	Bitmap GetDrawingTarget();
	void ResetDrawingTarget();
	void HoldBitmapDrawing(bool hold);

	void DrawView(const View& view);
	void DrawView(float x, float y, const View& view);

	void DrawSprite(const Sprite& sprite, int subimage, float x, float y);
	void DrawSprite(const Sprite& sprite, int subimage, float x, float y, const Color& blend, float xscale, float yscale, float angle);

	void DrawBitmap(const Bitmap& bitmap, float x, float y);
	void DrawBitmap(const Bitmap& bitmap, float x, float y, float xscale, float yscale);

	const Transform& GetTransform();
	void SetTransform(const Transform& transform);

}