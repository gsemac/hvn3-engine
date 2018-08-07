#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/sprites/Sprite.h"
#include "hvn3/utility/UTF8String.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

namespace hvn3 {

	namespace Graphics {

		// Helper functions
		int ConvertBlendOperation(BlendOperation operation) {

			switch (operation) {
			case BlendOperation::Subtract:
			case BlendOperation::SourceMinusDestination:
				return ALLEGRO_SRC_MINUS_DEST;
				break;
			case BlendOperation::DestinationMinusSource:
				return ALLEGRO_DEST_MINUS_SRC;
				break;
			case BlendOperation::Normal:
			case BlendOperation::Invert:
			case BlendOperation::Add:
			default:
				return ALLEGRO_ADD;
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

		void Graphics::DrawRectangle(const RectangleF& rect, const Color& color, float thickness) {
			DrawRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, thickness);
		}
		void Graphics::DrawRectangle(const PointF& position, const SizeF& size, const Color& color, float thickness) {
			DrawRectangle(position.x, position.y, size.width, size.height, color, thickness);
		}
		void Graphics::DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) {

			PrepareDrawingSurface();

			al_draw_rectangle(
				x + 1.0f,
				y + 1.0f,
				x + width,
				y + height,
				System::AllegroAdapter::ToColor(color),
				thickness
				);

		}
		void Graphics::DrawSolidRectangle(const RectangleF& rect, const Color& color) {
			DrawSolidRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color);
		}
		void Graphics::DrawSolidRectangle(const PointF& position, const SizeF& size, const Color& color) {
			DrawSolidRectangle(position.x, position.y, size.width, size.height, color);
		}
		void Graphics::DrawSolidRectangle(float x, float y, float width, float height, const Color& color) {

			PrepareDrawingSurface();

			al_draw_filled_rectangle(
				x,
				y,
				x + width,
				y + height,
				System::AllegroAdapter::ToColor(color)
				);

		}

		void Graphics::DrawRoundRectangle(const RectangleF& rect, const Color& color, float radius, float thickness) {

			DrawRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius, thickness);

		}
		void Graphics::DrawRoundRectangle(const PointF& position, const SizeF& size, const Color& color, float radius, float thickness) {
			DrawRoundRectangle(RectangleF(position, size), color, radius, thickness);
		}
		void Graphics::DrawRoundRectangle(float x, float y, float width, float height, const Color& color, float radius, float thickness) {

			PrepareDrawingSurface();

			// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
			al_draw_rounded_rectangle(x + 0.5f, y + 0.5f, x + width + 0.5f, y + height + 0.5f, radius, radius, System::AllegroAdapter::ToColor(color), thickness);

		}
		void Graphics::DrawSolidRoundRectangle(const RectangleF& rect, const Color& color, float radius) {

			DrawSolidRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius);

		}
		void Graphics::DrawSolidRoundRectangle(const PointF& position, const SizeF& size, const Color& color, float radius) {
			DrawSolidRoundRectangle(RectangleF(position, size), color, radius);
		}
		void Graphics::DrawSolidRoundRectangle(float x, float y, float width, float height, const Color& color, float radius) {

			PrepareDrawingSurface();

			// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
			al_draw_filled_rounded_rectangle(x, y, x + width, y + height, radius, radius, System::AllegroAdapter::ToColor(color));

		}

		void Graphics::DrawLine(const LineF& line) {

			DrawLine(line, Color::Black, 1.0f);

		}
		void Graphics::DrawLine(const LineF& line, const Color& color, float thickness) {

			DrawLine(line.First(), line.Second(), color, thickness);

		}
		void Graphics::DrawLine(const PointF& p1, const PointF& p2, const Color& color, float thickness) {

			DrawLine(p1.X(), p1.Y(), p2.X(), p2.Y(), color, thickness);

		}
		void Graphics::DrawLine(float x1, float y1, float x2, float y2, const Color& color, float thickness) {

			PrepareDrawingSurface();

			al_draw_line(x1, y1, x2, y2, System::AllegroAdapter::ToColor(color), thickness);

		}
		void Graphics::DrawLine(float x1, float y1, float x2, float y2, const Pen& pen) {

			PrepareDrawingSurface();

			if (Math::IsZero(pen.Width()))
				return;

			if (pen.DashStyle() == DashStyle::Solid) {

				DrawLine(x1, y1, x2, y2, pen.Color(), pen.Width());

			}
			else if (pen.DashStyle() == DashStyle::Dash) {

				bool dash = true;
				bool exit = false;
				size_t pattern_index = 0;

				PointF p_begin(x1, y1);
				PointF p_end(x2, y2);
				PointF p_next_begin = p_begin;

				if (Math::IsZero(Math::Geometry::PointDistance(p_begin, p_end)))
					return;

				while (!exit) {

					float gap_size = pen.Width();

					// If the user has specified a dash pattern, use that to determine the gap size.
					if (pen.DashPattern().size() > 0) {
						gap_size = pen.DashPattern()[pattern_index];
						if (++pattern_index >= pen.DashPattern().size())
							pattern_index = 0;
					}
					else
						// By default, have the dash length twice that of the space length.
						if (dash)
							gap_size *= 2.0f;

					PointF p_next_end = Math::Geometry::PointInDirection(p_next_begin, p_end, gap_size);
		
					// If we have surpassed the ending point, we can stop drawing the line after the next point.
					if ((Math::Abs(p_next_end.x) > Math::Abs(p_end.x)) || (Math::Abs(p_next_end.y) > Math::Abs(p_end.y))) {
						p_next_end = p_end;
						exit = true;
					}

					if (dash)
						DrawLine(p_next_begin.x, p_next_begin.y, p_next_end.x, p_next_end.y, pen.Color(), pen.Width());

					p_next_begin = p_next_end;
					dash = !dash;

				}

			}

		}

		void Graphics::DrawPoint(const PointF& point, const Color& color) {

			DrawPoint(point.X(), point.Y(), color);

		}
		void Graphics::DrawPoint(float x, float y, const Color& color) {

			PrepareDrawingSurface();

			al_put_pixel(x, y, System::AllegroAdapter::ToColor(color));

		}

		void Graphics::DrawCircle(const PointF& point, float radius, const Color& color, float thickness) {

			DrawCircle(point.X(), point.Y(), radius, color, thickness);

		}
		void Graphics::DrawCircle(float x, float y, float radius, const Color& color, float thickness) {

			PrepareDrawingSurface();

			al_draw_circle(
				x,
				y,
				radius,
				System::AllegroAdapter::ToColor(color),
				thickness
				);

		}
		void Graphics::DrawCircle(const CircleF& circle, const Color& color, float thickness) {

			DrawCircle(circle.X(), circle.Y(), circle.Radius(), color, thickness);

		}
		void Graphics::DrawSolidCircle(const PointF& point, float radius, const Color& color) {

			DrawSolidCircle(point.X(), point.Y(), radius, color);

		}
		void Graphics::DrawSolidCircle(float x, float y, float radius, const Color& color) {

			PrepareDrawingSurface();

			al_draw_filled_circle(
				x,
				y,
				radius,
				System::AllegroAdapter::ToColor(color)
				);

		}
		void Graphics::DrawSolidCircle(const CircleF& circle, const Color& color) {

			DrawSolidCircle(circle.X(), circle.Y(), circle.Radius(), color);

		}

		void Graphics::Clear(const Color& color) {

			PrepareDrawingSurface();

			al_clear_to_color(System::AllegroAdapter::ToColor(color));

		}

		void Graphics::DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) {

			PrepareDrawingSurface();

			if (HasFlag(alignment, Alignment::Middle))
				y -= font.Ascent() / 2.0f;
			else if (HasFlag(alignment, Alignment::Bottom))
				y -= font.Ascent();

			al_draw_text(System::AllegroAdapter::ToFont(font), System::AllegroAdapter::ToColor(color), x, y, System::AllegroAdapter::ToAlignmentFlags(alignment), text);

		}
		void Graphics::DrawText(float x, float y, const std::string& text, const Font& font, const Color& color) {

			DrawText(x, y, text.c_str(), font, color);

		}
		void Graphics::DrawText(float x, float y, const String& text, const Font& font, const Color& color, Alignment alignment) {

			PrepareDrawingSurface();

			if (HasFlag(alignment, Alignment::Middle))
				y -= font.Ascent() / 2.0f;
			else if(HasFlag(alignment, Alignment::Bottom))
				y -= font.Ascent();

			al_draw_ustr(System::AllegroAdapter::ToFont(font), System::AllegroAdapter::ToColor(color), x, y, System::AllegroAdapter::ToAlignmentFlags(alignment), text.AlPtr());

		}

		void Graphics::DrawSprite(float x, float y, const Sprite& sprite, int subimage) {

			PrepareDrawingSurface();

			al_draw_bitmap(
				System::AllegroAdapter::ToBitmap((sprite)[subimage]),
				x + sprite.Origin().X(),
				y + sprite.Origin().Y(),
				NULL
				);

		}
		void Graphics::DrawSprite(float x, float y, const Sprite& sprite, int subimage, float xscale, float yscale, float angle, const Color& blend) {

			PrepareDrawingSurface();

			al_draw_tinted_scaled_rotated_bitmap(
				System::AllegroAdapter::ToBitmap((sprite)[subimage]),
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

		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap) {

			PrepareDrawingSurface();

			al_draw_bitmap(
				System::AllegroAdapter::ToBitmap(bitmap),
				x,
				y,
				NULL
				);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float xscale, float yscale) {

			PrepareDrawingSurface();

			al_draw_scaled_bitmap(
				System::AllegroAdapter::ToBitmap(bitmap),
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
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float xscale, float yscale, const PointF& origin, float angle) {

			PrepareDrawingSurface();

			al_draw_scaled_rotated_bitmap(
				System::AllegroAdapter::ToBitmap(bitmap),
				origin.X(),
				origin.Y(),
				x,
				y,
				xscale,
				yscale,
				Math::DegreesToRadians(angle),
				NULL
				);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, const RectangleF& region) {

			PrepareDrawingSurface();

			al_draw_bitmap_region(System::AllegroAdapter::ToBitmap(bitmap), region.X(), region.Y(), region.Width(), region.Height(), x, y, NULL);

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, const Color& tint) {

			PrepareDrawingSurface();

			al_draw_tinted_bitmap(System::AllegroAdapter::ToBitmap(bitmap), al_map_rgba_f(tint.Alphaf(), tint.Alphaf(), tint.Alphaf(), tint.Alphaf()), x, y, NULL);

		}

		void Graphics::SetClip(const RectangleF& rect) {

			SetClip(rect.X(), rect.Y(), rect.Width(), rect.Height());

		}
		void Graphics::SetClip(int x, int y, int width, int height) {

			_clipping_region = RectangleF(x, y, width, height);

			if (IsActiveSurface())
				ApplyClip();

		}
		RectangleF Graphics::Clip() const {

			return _clipping_region;

		}
		void Graphics::ResetClip() {

			SetClip(0, 0, _surface.Width(), _surface.Height());

		}

		void Graphics::HoldBitmapDrawing(bool hold) {

			al_hold_bitmap_drawing(hold);

		}
		bool Graphics::BitmapDrawingHeld() const {

			return al_is_bitmap_drawing_held();

		}

		GraphicsState Graphics::Save() const {

			return GraphicsState(*this);

		}
		void Graphics::Restore(const GraphicsState& state) {

			_clipping_region = state.__clip;
			_transform = state.__transform;

			if (IsActiveSurface()) {
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
				SetBlendMode(BlendOperation::Add, BlendMode::Zero, BlendMode::InverseSourceColor);
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

			throw System::NotImplementedException();

		}
		void Graphics::ResetBlendMode() {

			SetBlendMode(BlendOperation::Normal);

		}

		void Graphics::PrepareDrawingSurface() {

			_surface._perform_pre_write_operations();

			// If this Object's drawing surface is not set as the current drawing surface, set it.
			if (!IsActiveSurface()) {
				al_set_target_bitmap(System::AllegroAdapter::ToBitmap(_surface));
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

		Graphics* Graphics::_last_to_draw = nullptr;

	}

}