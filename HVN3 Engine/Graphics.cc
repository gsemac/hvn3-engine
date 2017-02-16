#include <stack>
#include "Graphics.h"
#include "Exception.h"

namespace Drawing {

	Graphics::Graphics(Bitmap& surface) :
		__surface(surface),
		__clipping_region(0.0f, 0.0f, surface.Width(), surface.Height()) {}
	Graphics::~Graphics() {
		
		al_set_target_bitmap(nullptr);

	}

	void Graphics::DrawRectangle(const Rectangle& rect, const Color& color, float thickness) {

		DrawRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, thickness);

	}
	void Graphics::DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) {

		PrepareDrawingSurface();

		al_draw_rectangle(
			x,
			y,
			x + width,
			y + height,
			color.AlPtr(),
			thickness
			);

	}
	void Graphics::DrawFilledRectangle(const Rectangle& rect, const Color& color) {

		DrawFilledRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color);

	}
	void Graphics::DrawFilledRectangle(float x, float y, float width, float height, const Color& color) {

		PrepareDrawingSurface();

		al_draw_filled_rectangle(
			x,
			y,
			x + width,
			y + height,
			color.AlPtr()
			);

	}

	void Graphics::DrawRoundRectangle(const Rectangle& rect, const Color& color, float radius, float thickness) {

		DrawRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius, thickness);

	}
	void Graphics::DrawRoundRectangle(float x, float y, float width, float height, const Color& color, float radius, float thickness) {

		PrepareDrawingSurface();

		// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
		al_draw_rounded_rectangle(x + 0.5f, y + 0.5f, x + width + 0.5f, y + height + 0.5f, radius, radius, color.AlPtr(), thickness);

	}
	void Graphics::DrawFilledRoundRectangle(const Rectangle& rect, const Color& color, float radius) {

		DrawFilledRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius);

	}
	void Graphics::DrawFilledRoundRectangle(float x, float y, float width, float height, const Color& color, float radius) {

		PrepareDrawingSurface();

		// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
		al_draw_filled_rounded_rectangle(x + 0.5f, y + 0.5f, x + width + 0.5f, y + height + 0.5f, radius, radius, color.AlPtr());

	}

	void Graphics::DrawLine(const Line& line) {

		DrawLine(line, Color::Black, 1.0f);

	}
	void Graphics::DrawLine(const Line& line, const Color& color, float thickness) {

		DrawLine(line.First(), line.Second(), color, thickness);

	}
	void Graphics::DrawLine(const Point& p1, const Point& p2, const Color& color, float thickness) {

		DrawLine(p1.X(), p1.Y(), p2.X(), p2.Y(), color, thickness);

	}
	void Graphics::DrawLine(float x1, float y1, float x2, float y2, const Color& color, float thickness) {

		PrepareDrawingSurface();

		al_draw_line(x1, y1, x2, y2, color.AlPtr(), thickness);

	}

	void Graphics::DrawPoint(const Point& point, const Color& color) {

		DrawPoint(point.X(), point.Y(), color);

	}
	void Graphics::DrawPoint(float x, float y, const Color& color) {

		PrepareDrawingSurface();

		al_put_pixel(x, y, color.AlPtr());

	}

	void Graphics::DrawCircle(const Point& point, float radius, const Color& color, float thickness) {

		DrawCircle(point.X(), point.Y(), radius, color, thickness);

	}
	void Graphics::DrawCircle(float x, float y, float radius, const Color& color, float thickness) {

		PrepareDrawingSurface();

		al_draw_circle(
			x,
			y,
			radius,
			color.AlPtr(),
			thickness
			);

	}
	void Graphics::DrawFilledCircle(const Point& point, float radius, const Color& color) {

		DrawFilledCircle(point.X(), point.Y(), radius, color);

	}
	void Graphics::DrawFilledCircle(float x, float y, float radius, const Color& color) {

		PrepareDrawingSurface();

		al_draw_filled_circle(
			x,
			y,
			radius,
			color.AlPtr()
			);

	}
	
	void Graphics::Clear(const Color& color) {

		PrepareDrawingSurface();

		al_clear_to_color(color.AlPtr());

	}

	void Graphics::DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) {

		PrepareDrawingSurface();

		al_draw_text(font.AlPtr(), color.AlPtr(), x, y, GetAllegroFlags(alignment), text);

	}
	void Graphics::DrawText(float x, float y, const std::string& text, const Font& font, const Color& color) {

		DrawText(x, y, text.c_str(), font, color);

	}
	void Graphics::DrawText(float x, float y, Utf8String& text, const Font& font, const Color& color, Alignment alignment) {

		PrepareDrawingSurface();

		al_draw_ustr(font.AlPtr(), color.AlPtr(), x, y, GetAllegroFlags(alignment), text.AlPtr());

	}

	void Graphics::DrawSprite(const Sprite& sprite, int subimage, float x, float y) {

		PrepareDrawingSurface();

		al_draw_bitmap(
			sprite[subimage % (int)sprite.Length()],
			x + sprite.Origin().X(),
			y + sprite.Origin().Y(),
			NULL
			);

	}
	void Graphics::DrawSprite(const Sprite& sprite, int subimage, float x, float y, const Color& blend, float xscale, float yscale, float angle) {

		PrepareDrawingSurface();

		al_draw_tinted_scaled_rotated_bitmap(
			sprite[subimage % (int)sprite.Length()],
			al_map_rgba_f(blend.Rf() * blend.Alphaf(), blend.Gf() * blend.Alphaf(), blend.Bf() * blend.Alphaf(), blend.Alphaf()),
			sprite.Origin().X(),
			sprite.Origin().Y(),
			x,
			y,
			xscale,
			yscale,
			angle,
			NULL
			);

	}

	void Graphics::DrawBitmap(const Bitmap& bitmap, float x, float y) {

		PrepareDrawingSurface();

		al_draw_bitmap(
			bitmap.AlPtr(),
			x,
			y,
			NULL
			);

	}
	void Graphics::DrawBitmap(const Bitmap& bitmap, float x, float y, float xscale, float yscale) {

		PrepareDrawingSurface();

		al_draw_scaled_bitmap(
			bitmap.AlPtr(),
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
	void Graphics::DrawBitmap(const Bitmap& bitmap, const Rectangle& region, float x, float y) {

		PrepareDrawingSurface();

		al_draw_bitmap_region(bitmap.AlPtr(), region.X(), region.Y(), region.Width(), region.Height(), x, y, NULL);

	}

	void Graphics::SetClip(const Rectangle& rect) {

		SetClip(rect.X(), rect.Y(), rect.Width(), rect.Height());

	}
	void Graphics::SetClip(int x, int y, int width, int height) {

		__clipping_region = Rectangle(x, y, width, height);

		if (IsActiveSurface())
			ApplyClip();

	}
	Rectangle Graphics::Clip() const {

		return __clipping_region;

	}
	void Graphics::ResetClip() {

		SetClip(0, 0, __surface.Width(), __surface.Height());

	}

	void Graphics::HoldBitmapDrawing(bool hold) {

		al_hold_bitmap_drawing(hold);

	}

	GraphicsState Graphics::Save() const {

		return GraphicsState(*this);

	}
	void Graphics::Restore(const GraphicsState& state) {

		__clipping_region = state.__clip;
		__transform = state.__transform;

		if (!IsActiveSurface()) {
			ApplyClip();
			ApplyTransform();
		}

	}

	void Graphics::SetTransform(const Transform& transform) {

		__transform = transform;

		if (IsActiveSurface())
			ApplyTransform();

	}
	const Transform& Graphics::GetTransform() const {

		return __transform;

	}
	void Graphics::ResetTransform() {

		__transform.Reset();

		if (IsActiveSurface())
			ApplyTransform();

	}

	void Graphics::PrepareDrawingSurface() {

		// If this Object's drawing surface is not set as the current drawing surface, set it.
		if (!IsActiveSurface()) {
			al_set_target_bitmap(__surface.AlPtr());
			ApplyClip();
			ApplyTransform();
		}

	}

	void Graphics::ApplyTransform() {

		al_use_transform(__transform.AlPtr());

	}
	void Graphics::ApplyClip() {
		
		al_set_clipping_rectangle(__clipping_region.X(), __clipping_region.Y(), __clipping_region.Width(), __clipping_region.Height());

	}
	bool Graphics::IsActiveSurface() const {
		
		return al_get_target_bitmap() == __surface.AlPtr();

	}
	int Graphics::GetAllegroFlags(Alignment value) const {

		int flags = 0;
	
		switch (value) {
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

		return flags;

	}

}