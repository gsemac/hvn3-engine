#include "Graphics.h"
#include "Exception.h"
#include "FrameworkAdapter.h"
#include "UTF8String.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

namespace hvn3 {

	namespace Drawing {

		// Helper functions
		int ConvertBlendOperation(BlendOperation operation) {

			switch (operation) {
			case BlendOperation::Normal:
			case BlendOperation::Invert:
			case BlendOperation::Add:
				return ALLEGRO_ADD;
				break;
			case BlendOperation::Subtract:
			case BlendOperation::SourceMinusDestination:
				return ALLEGRO_SRC_MINUS_DEST;
				break;
			case BlendOperation::DestinationMinusSource:
				return ALLEGRO_DEST_MINUS_SRC;
				break;
			}

		}
		int ConvertBlendMode(BlendMode mode) {

			switch (mode) {
			case BlendMode::Zero:
				return ALLEGRO_ZERO;
			case BlendMode::One:
				return ALLEGRO_ONE;
			case BlendMode::Alpha:
				return ALLEGRO_ALPHA;
			case BlendMode::InverseAlpha:
				return ALLEGRO_INVERSE_ALPHA;
			case BlendMode::SourceColor:
				return ALLEGRO_SRC_COLOR;
			case BlendMode::DestinationColor:
				return ALLEGRO_DEST_COLOR;
			case BlendMode::InverseSourceColor:
				return ALLEGRO_INVERSE_SRC_COLOR;
			case BlendMode::InverseDestinationColor:
				return ALLEGRO_INVERSE_DEST_COLOR;
			case BlendMode::ConstColor:
				return ALLEGRO_CONST_COLOR;
			case BlendMode::InverseConstColor:
				return ALLEGRO_INVERSE_CONST_COLOR;
			}

			return 0;

		}

		Graphics::Graphics(Bitmap& surface) :
			_surface(surface),
			_clipping_region(0.0f, 0.0f, surface.Width(), surface.Height()) {
		}
		Graphics::~Graphics() {

			if (_last_to_draw == this)
				_last_to_draw = nullptr;

		}

		void Graphics::DrawRectangle(const Rectangle<float>& rect, const Color& color, float thickness) {

			DrawRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, thickness);

		}
		void Graphics::DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) {

			PrepareDrawingSurface();

			al_draw_rectangle(
				x + 1.0f,
				y + 1.0f,
				x + width,
				y + height,
				FrameworkAdapter::ToColor(color),
				thickness
				);

		}
		void Graphics::DrawFilledRectangle(const Rectangle<float>& rect, const Color& color) {

			DrawFilledRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color);

		}
		void Graphics::DrawFilledRectangle(float x, float y, float width, float height, const Color& color) {

			PrepareDrawingSurface();

			al_draw_filled_rectangle(
				x,
				y,
				x + width,
				y + height,
				FrameworkAdapter::ToColor(color)
				);

		}

		void Graphics::DrawRoundRectangle(const Rectangle<float>& rect, const Color& color, float radius, float thickness) {

			DrawRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius, thickness);

		}
		void Graphics::DrawRoundRectangle(float x, float y, float width, float height, const Color& color, float radius, float thickness) {

			PrepareDrawingSurface();

			// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
			al_draw_rounded_rectangle(x + 0.5f, y + 0.5f, x + width + 0.5f, y + height + 0.5f, radius, radius, FrameworkAdapter::ToColor(color), thickness);

		}
		void Graphics::DrawFilledRoundRectangle(const Rectangle<float>& rect, const Color& color, float radius) {

			DrawFilledRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius);

		}
		void Graphics::DrawFilledRoundRectangle(float x, float y, float width, float height, const Color& color, float radius) {

			PrepareDrawingSurface();

			// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
			al_draw_filled_rounded_rectangle(x + 0.5f, y + 0.5f, x + width + 0.5f, y + height + 0.5f, radius, radius, FrameworkAdapter::ToColor(color));

		}

		void Graphics::DrawLine(const Line<float>& line) {

			DrawLine(line, Color::Black, 1.0f);

		}
		void Graphics::DrawLine(const Line<float>& line, const Color& color, float thickness) {

			DrawLine(line.First(), line.Second(), color, thickness);

		}
		void Graphics::DrawLine(const Point2d<float>& p1, const Point2d<float>& p2, const Color& color, float thickness) {

			DrawLine(p1.X(), p1.Y(), p2.X(), p2.Y(), color, thickness);

		}
		void Graphics::DrawLine(float x1, float y1, float x2, float y2, const Color& color, float thickness) {

			PrepareDrawingSurface();

			al_draw_line(x1, y1, x2, y2, FrameworkAdapter::ToColor(color), thickness);

		}

		void Graphics::DrawPoint(const Point2d<float>& point, const Color& color) {

			DrawPoint(point.X(), point.Y(), color);

		}
		void Graphics::DrawPoint(float x, float y, const Color& color) {

			PrepareDrawingSurface();

			al_put_pixel(x, y, FrameworkAdapter::ToColor(color));

		}

		void Graphics::DrawCircle(const Point2d<float>& point, float radius, const Color& color, float thickness) {

			DrawCircle(point.X(), point.Y(), radius, color, thickness);

		}
		void Graphics::DrawCircle(float x, float y, float radius, const Color& color, float thickness) {

			PrepareDrawingSurface();

			al_draw_circle(
				x,
				y,
				radius,
				FrameworkAdapter::ToColor(color),
				thickness
				);

		}
		void Graphics::DrawFilledCircle(const Point2d<float>& point, float radius, const Color& color) {

			DrawFilledCircle(point.X(), point.Y(), radius, color);

		}
		void Graphics::DrawFilledCircle(float x, float y, float radius, const Color& color) {

			PrepareDrawingSurface();

			al_draw_filled_circle(
				x,
				y,
				radius,
				FrameworkAdapter::ToColor(color)
				);

		}

		void Graphics::Clear(const Color& color) {

			PrepareDrawingSurface();

			al_clear_to_color(FrameworkAdapter::ToColor(color));

		}

		void Graphics::DrawText(float x, float y, const char* text, const Font* font, const Color& color, Alignment alignment) {

			PrepareDrawingSurface();

			al_draw_text(font->AlPtr(), FrameworkAdapter::ToColor(color), x, y, GetAllegroFlags(alignment), text);

		}
		void Graphics::DrawText(float x, float y, const std::string& text, const Font* font, const Color& color) {

			DrawText(x, y, text.c_str(), font, color);

		}
		void Graphics::DrawText(float x, float y, const String& text, const Font* font, const Color& color, Alignment alignment) {

			PrepareDrawingSurface();

			al_draw_ustr(font->AlPtr(), FrameworkAdapter::ToColor(color), x, y, GetAllegroFlags(alignment), text.AlPtr());

		}

		void Graphics::DrawSprite(float x, float y, const Sprite* sprite, int subimage) {

			PrepareDrawingSurface();

			al_draw_bitmap(
				(*sprite)[subimage].AlPtr(),
				x + sprite->Origin().X(),
				y + sprite->Origin().Y(),
				NULL
				);

		}
		void Graphics::DrawSprite(float x, float y, const Sprite* sprite, int subimage, float xscale, float yscale, float angle, const Color& blend) {

			PrepareDrawingSurface();

			al_draw_tinted_scaled_rotated_bitmap(
				(*sprite)[subimage].AlPtr(),
				al_map_rgba_f(blend.Rf() * blend.Alphaf(), blend.Gf() * blend.Alphaf(), blend.Bf() * blend.Alphaf(), blend.Alphaf()),
				sprite->Origin().X(),
				sprite->Origin().Y(),
				x,
				y,
				xscale,
				yscale,
				angle,
				NULL
				);

		}

		void Graphics::DrawBitmap(float x, float y, const Bitmap* bitmap) {

			PrepareDrawingSurface();

			al_draw_bitmap(
				bitmap->AlPtr(),
				x,
				y,
				NULL
				);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap* bitmap, float xscale, float yscale) {

			PrepareDrawingSurface();

			al_draw_scaled_bitmap(
				bitmap->AlPtr(),
				0.0f,
				0.0f,
				bitmap->Width(),
				bitmap->Height(),
				x,
				y,
				bitmap->Width() * xscale,
				bitmap->Height() * yscale,
				NULL
				);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap* bitmap, float xscale, float yscale, const Point2d<float>& origin, float angle) {

			PrepareDrawingSurface();

			al_draw_scaled_rotated_bitmap(
				bitmap->AlPtr(),
				origin.X(),
				origin.Y(),
				x,
				y,
				xscale,
				yscale,
				DegreesToRadians(angle),
				NULL
				);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap* bitmap, const Rectangle<float>& region) {

			PrepareDrawingSurface();

			al_draw_bitmap_region(bitmap->AlPtr(), region.X(), region.Y(), region.Width(), region.Height(), x, y, NULL);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap* bitmap, const Color& tint) {

			PrepareDrawingSurface();

			al_draw_tinted_bitmap(bitmap->AlPtr(), al_map_rgba_f(tint.Alphaf(), tint.Alphaf(), tint.Alphaf(), tint.Alphaf()), x, y, NULL);

		}

		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap) {

			DrawBitmap(x, y, &bitmap);

		}

		void Graphics::SetClip(const Rectangle<float>& rect) {

			SetClip(rect.X(), rect.Y(), rect.Width(), rect.Height());

		}
		void Graphics::SetClip(int x, int y, int width, int height) {

			_clipping_region = Rectangle<float>(x, y, width, height);

			if (IsActiveSurface())
				ApplyClip();

		}
		Rectangle<float> Graphics::Clip() const {

			return _clipping_region;

		}
		void Graphics::ResetClip() {

			SetClip(0, 0, _surface.Width(), _surface.Height());

		}

		void Graphics::HoldBitmapDrawing(bool hold) {

			al_hold_bitmap_drawing(hold);

		}

		GraphicsState Graphics::Save() const {

			return GraphicsState(*this);

		}
		void Graphics::Restore(const GraphicsState& state) {

			_clipping_region = state.__clip;
			_transform = state.__transform;

			if (!IsActiveSurface()) {
				ApplyClip();
				ApplyTransform();
			}

		}

		void Graphics::SetTransform(const Transform& transform) {

			_transform = transform;

			if (IsActiveSurface())
				ApplyTransform();

		}
		const Transform& Graphics::GetTransform() const {

			return _transform;

		}
		void Graphics::ResetTransform() {

			_transform.Reset();

			if (IsActiveSurface())
				ApplyTransform();

		}

		void Graphics::SetBlendMode(BlendOperation operation) {

			switch (operation) {

			case BlendOperation::Normal:
				SetBlendMode(BlendOperation::Add, BlendMode::One, BlendMode::InverseAlpha);
				break;

			case BlendOperation::Add:
				SetBlendMode(BlendOperation::Add, BlendMode::SourceColor, BlendMode::One);
				break;

			case BlendOperation::Subtract:
				SetBlendMode(BlendOperation::Subtract, BlendMode::Zero, BlendMode::InverseSourceColor);
				break;

			case BlendOperation::Max:
				SetBlendMode(BlendOperation::Max, BlendMode::SourceColor, BlendMode::InverseSourceColor);
				break;

			case BlendOperation::Invert:
				SetBlendMode(BlendOperation::Add, BlendMode::InverseDestinationColor, BlendMode::InverseSourceColor);
				break;

			}

		}
		void Graphics::SetBlendMode(BlendOperation operation, BlendMode source, BlendMode destination) {

			int op = ConvertBlendOperation(operation);
			int src = ConvertBlendMode(source);
			int dest = ConvertBlendMode(destination);
			
			al_set_blender(op, src, dest);

		}
		void Graphics::SetBlendMode(BlendOperation operation, const Color& source, const Color& destination) {



		}
		void Graphics::ResetBlendMode() {

			SetBlendMode(BlendOperation::Normal);

		}
		 
		void Graphics::PrepareDrawingSurface() {

			// If this Object's drawing surface is not set as the current drawing surface, set it.
			if (!IsActiveSurface()) {
				al_set_target_bitmap(_surface.AlPtr());
				ApplyClip();
				ApplyTransform();
				_last_to_draw = this;
			}

		}

		void Graphics::ApplyTransform() {

			al_use_transform((ALLEGRO_TRANSFORM*)&_transform);

		}
		void Graphics::ApplyClip() {

			al_set_clipping_rectangle(_clipping_region.X(), _clipping_region.Y(), _clipping_region.Width(), _clipping_region.Height());

		}
		bool Graphics::IsActiveSurface() const {

			return (_last_to_draw == this);

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

		Graphics* Graphics::_last_to_draw = nullptr;

	}

}