#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/sprites/Sprite.h"
#include "hvn3/utility/UTF8String.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <cassert>
#include <cmath>
#include <limits>

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
			_canvas(&surface),
			_path_canvas(nullptr),
			_clipping_region(0.0f, 0.0f, surface.Width(), surface.Height()) {
		}
		Graphics::Graphics(GraphicsPath& path) :
			_canvas(nullptr),
			_path_canvas(&path) {
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

			x += 1.0f;
			y += 1.0f;
			width -= 1.0f;
			height -= 1.0f;

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_rectangle(
					x,
					y,
					x + width,
					y + height,
					System::AllegroAdapter::ToColor(color),
					thickness
				);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, width, height);

		}
		void Graphics::DrawSolidRectangle(const RectangleF& rect, const Color& color) {
			DrawSolidRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color);
		}
		void Graphics::DrawSolidRectangle(const PointF& position, const SizeF& size, const Color& color) {
			DrawSolidRectangle(position.x, position.y, size.width, size.height, color);
		}
		void Graphics::DrawSolidRectangle(float x, float y, float width, float height, const Color& color) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_filled_rectangle(
					x,
					y,
					x + width,
					y + height,
					System::AllegroAdapter::ToColor(color)
				);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, width, height);

		}

		void Graphics::DrawRoundRectangle(const RectangleF& rect, const Color& color, float radius, float thickness) {

			DrawRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius, thickness);

		}
		void Graphics::DrawRoundRectangle(const PointF& position, const SizeF& size, const Color& color, float radius, float thickness) {
			DrawRoundRectangle(RectangleF(position, size), color, radius, thickness);
		}
		void Graphics::DrawRoundRectangle(float x, float y, float width, float height, const Color& color, float radius, float thickness) {

			// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_rounded_rectangle(x + 0.5f, y + 0.5f, x + width - 0.5f, y + height - 0.5f, radius, radius, System::AllegroAdapter::ToColor(color), thickness);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x + 0.5f, y + 0.5f, x + width - 0.5f, y + height - 0.5f);

		}
		void Graphics::DrawSolidRoundRectangle(const RectangleF& rect, const Color& color, float radius) {

			DrawSolidRoundRectangle(rect.X(), rect.Y(), rect.Width(), rect.Height(), color, radius);

		}
		void Graphics::DrawSolidRoundRectangle(const PointF& position, const SizeF& size, const Color& color, float radius) {
			DrawSolidRoundRectangle(RectangleF(position, size), color, radius);
		}
		void Graphics::DrawSolidRoundRectangle(float x, float y, float width, float height, const Color& color, float radius) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				// Note: 0.5 is added to each coordinate to fix the uneven corners drawn by Allegro.
				al_draw_filled_rounded_rectangle(x, y, x + width, y + height, radius, radius, System::AllegroAdapter::ToColor(color));

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, width, height);

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

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_line(x1, y1, x2, y2, System::AllegroAdapter::ToColor(color), thickness);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddLine(x1, y1, x2, y2);

		}
		void Graphics::DrawLine(float x1, float y1, float x2, float y2, const Pen& pen) {

			_makeThisActiveInstance(true);

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
		void Graphics::DrawLine(float x1, float y1, float x2, float y2, const LinearGradientBrush& brush, float thickness) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_line(x1, y1, x2, y2, System::AllegroAdapter::ToColor(brush.StartColor()), System::AllegroAdapter::ToColor(brush.EndColor()), thickness);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddLine(x1, y1, x2, y2);

		}

		void Graphics::DrawPixel(const PointF& point, const Color& color) {

			DrawPixel(point.X(), point.Y(), color);

		}
		void Graphics::DrawPixel(float x, float y, const Color& color) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_pixel(x, y, System::AllegroAdapter::ToColor(color));

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddPoint(x, y, GraphicsPath::VertexTag::Point);

		}

		void Graphics::DrawCircle(const PointF& point, float radius, const Color& color, float thickness) {

			DrawCircle(point.X(), point.Y(), radius, color, thickness);

		}
		void Graphics::DrawCircle(float x, float y, float radius, const Color& color, float thickness) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_circle(
					x,
					y,
					radius,
					System::AllegroAdapter::ToColor(color),
					thickness
				);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddCircle(x, y, radius);

		}
		void Graphics::DrawCircle(const CircleF& circle, const Color& color, float thickness) {

			DrawCircle(circle.X(), circle.Y(), circle.Radius(), color, thickness);

		}
		void Graphics::DrawSolidCircle(const PointF& point, float radius, const Color& color) {

			DrawSolidCircle(point.X(), point.Y(), radius, color);

		}
		void Graphics::DrawSolidCircle(float x, float y, float radius, const Color& color) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_filled_circle(
					x,
					y,
					radius,
					System::AllegroAdapter::ToColor(color)
				);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddCircle(x, y, radius);

		}
		void Graphics::DrawSolidCircle(const CircleF& circle, const Color& color) {

			DrawSolidCircle(circle.X(), circle.Y(), circle.Radius(), color);

		}

		void Graphics::Clear(const Color& color) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_clear_to_color(System::AllegroAdapter::ToColor(color));

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(Clip().X(), Clip().Y(), Clip().Width(), Clip().Height());

		}
		void Graphics::Fill(const Color& color) {

			if (_canvas != nullptr) {

				Transform trans = GetTransform();

				ResetTransform();

				DrawSolidRectangle(0.0f, 0.0f, static_cast<float>(_canvas->Width()), static_cast<float>(_canvas->Height()), color);

				SetTransform(trans);

			}
			else
				Clear(color);
		}

		void Graphics::DrawText(float x, float y, const std::string& text) {
			DrawText(x, y, text, Color::Black);
		}
		void Graphics::DrawText(float x, float y, const std::string& text, const Color& color) {
			DrawText(x, y, text, Font::BuiltIn(), color);
		}
		void Graphics::DrawText(float x, float y, const std::string& text, const Font& font, const Color& color) {

			DrawText(x, y, text.c_str(), font, color);

		}
		void Graphics::DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				if (HasFlag(alignment, Alignment::Middle))
					y -= font.Ascent() / 2.0f;
				else if (HasFlag(alignment, Alignment::Bottom))
					y -= font.Ascent();

				al_draw_text(System::AllegroAdapter::ToFont(font), System::AllegroAdapter::ToColor(color), x, y, System::AllegroAdapter::ToAlignmentFlags(alignment), text);

			}

			else if (_path_canvas != nullptr) {

				int bbx, bby, bbw, bbh;
				al_get_text_dimensions(System::AllegroAdapter::ToFont(font), text, &bbx, &bby, &bbw, &bbh);

				_path_canvas->AddRectangle(
					static_cast<float>(bbx),
					static_cast<float>(bby),
					static_cast<float>(bbw),
					static_cast<float>(bbh));

			}

		}
		void Graphics::DrawText(float x, float y, const String& text, const Font& font, const Color& color, Alignment alignment) {

			// Apply alignment flags.

			if (HasFlag(alignment, Alignment::Middle))
				y -= font.Height() / 2.0f;
			else if (HasFlag(alignment, Alignment::Bottom))
				y -= font.Height();

			// If drawing a monochrome (pixel) font, round the drawing coordinates.

			if (HasFlag(font.Flags(), FontFlags::Monochrome)) {

				x = Math::Round(x);
				y = Math::Round(y);

			}

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_ustr(System::AllegroAdapter::ToFont(font), System::AllegroAdapter::ToColor(color), x, y, System::AllegroAdapter::ToAlignmentFlags(alignment), System::AllegroAdapter::ToUStr(text));

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, text.Width(font), text.Height(font));

		}

		void Graphics::DrawSprite(float x, float y, const Sprite& sprite, int subimage) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_bitmap(
					System::AllegroAdapter::ToBitmap((sprite)[subimage]),
					x + sprite.Origin().X(),
					y + sprite.Origin().Y(),
					NULL
				);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x - sprite.Origin().X(), y - sprite.Origin().Y(), sprite.Width(), sprite.Height());

		}
		void Graphics::DrawSprite(float x, float y, const Sprite& sprite, int subimage, float xscale, float yscale, float angle, const Color& blend) {

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_tinted_scaled_rotated_bitmap(
					System::AllegroAdapter::ToBitmap((sprite)[subimage]),
					al_map_rgba_f(blend.Rf() * blend.Alphaf(), blend.Gf() * blend.Alphaf(), blend.Bf() * blend.Alphaf(), blend.Alphaf()),
					sprite.Origin().X(),
					sprite.Origin().Y(),
					x,
					y,
					xscale,
					yscale,
					Math::DegreesToRadians(-angle),
					NULL
				);

			}

			else if (_path_canvas != nullptr) {

				Transform transform;
				transform.Rotate(sprite.Origin(), angle);
				transform.Scale(xscale, yscale);

				auto vertices = RectangleF(x - sprite.Origin().x, y - sprite.Origin().y, sprite.Width(), sprite.Height()).Transform(transform);

				_path_canvas->AddPoints(vertices.begin(), vertices.end(), GraphicsPath::VertexTag::Polygon);

			}

		}

		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_bitmap(
					System::AllegroAdapter::ToBitmap(bitmap),
					x,
					y,
					NULL
				);

			}

			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, bitmap.Width(), bitmap.Height());

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float xscale, float yscale) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

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

			else if (_path_canvas != nullptr) {

				Transform transform;
				transform.Scale(xscale, yscale);

				auto vertices = RectangleF(x, y, bitmap.Width(), bitmap.Height()).Transform(transform);

				_path_canvas->AddPoints(vertices.begin(), vertices.end(), GraphicsPath::VertexTag::Polygon);

			}

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float xscale, float yscale, const Color& tint) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_tinted_scaled_bitmap(
					System::AllegroAdapter::ToBitmap(bitmap),
					System::AllegroAdapter::ToColor(tint),
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

			else if (_path_canvas != nullptr) {

				Transform transform;
				transform.Scale(xscale, yscale);

				auto vertices = RectangleF(x, y, bitmap.Width(), bitmap.Height()).Transform(transform);

				_path_canvas->AddPoints(vertices.begin(), vertices.end(), GraphicsPath::VertexTag::Polygon);

			}

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float xscale, float yscale, const PointF& origin, float angle) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_scaled_rotated_bitmap(
					System::AllegroAdapter::ToBitmap(bitmap),
					origin.X(),
					origin.Y(),
					x,
					y,
					xscale,
					yscale,
					Math::DegreesToRadians(-angle),
					NULL
				);

			}

			else if (_path_canvas != nullptr) {

				Transform transform;
				transform.Rotate(origin, angle);
				transform.Scale(xscale, yscale);

				auto vertices = RectangleF(x, y, bitmap.Width(), bitmap.Height()).Transform(transform);

				_path_canvas->AddPoints(vertices.begin(), vertices.end(), GraphicsPath::VertexTag::Polygon);

			}

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, float xscale, float yscale, const PointF& origin, float angle, const Color& tint) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_tinted_scaled_rotated_bitmap(
					System::AllegroAdapter::ToBitmap(bitmap),
					System::AllegroAdapter::ToColor(tint),
					origin.X(),
					origin.Y(),
					x,
					y,
					xscale,
					yscale,
					Math::DegreesToRadians(angle),
					NULL);

			}

			else if (_path_canvas != nullptr) {

				Transform transform;
				transform.Rotate(origin, angle);
				transform.Scale(xscale, yscale);

				auto vertices = RectangleF(x, y, bitmap.Width(), bitmap.Height()).Transform(transform);

				_path_canvas->AddPoints(vertices.begin(), vertices.end(), GraphicsPath::VertexTag::Polygon);

			}

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, const RectangleF& region) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_bitmap_region(System::AllegroAdapter::ToBitmap(bitmap), region.X(), region.Y(), region.Width(), region.Height(), x, y, NULL);

			}
			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, region.Width(), region.Height());

		}
		void Graphics::DrawBitmap(float x, float y, const Bitmap& bitmap, const Color& tint) {

			assert(static_cast<bool>(bitmap));

			if (_canvas != nullptr) {

				_makeThisActiveInstance(true);

				al_draw_tinted_bitmap(System::AllegroAdapter::ToBitmap(bitmap), al_map_rgba_f(tint.Alphaf(), tint.Alphaf(), tint.Alphaf(), tint.Alphaf()), x, y, NULL);

			}
			else if (_path_canvas != nullptr)
				_path_canvas->AddRectangle(x, y, bitmap.Width(), bitmap.Height());

		}

		void Graphics::SetClip(const RectangleF& rect) {

			SetClip(rect.X(), rect.Y(), rect.Width(), rect.Height());

		}
		void Graphics::SetClip(int x, int y, int width, int height) {

			_clipping_region = RectangleF(x, y, width, height);

			if (_isActiveInstance())
				_applyClip();

		}
		RectangleF Graphics::Clip() const {

			return _clipping_region;

		}
		void Graphics::ResetClip() {

			if (_canvas != nullptr)
				SetClip(0, 0, _canvas->Width(), _canvas->Height());
			else
				SetClip(0, 0, std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

		}

		void Graphics::SetBitmapDrawingHeld(bool value) {

			_makeThisActiveInstance(false);

			al_hold_bitmap_drawing(value);

		}
		bool Graphics::BitmapDrawingHeld() const {

			_makeThisActiveInstance(false);

			return al_is_bitmap_drawing_held();

		}

		GraphicsState Graphics::Save() const {

			return GraphicsState(*this);

		}
		void Graphics::Restore(const GraphicsState& state) {

			_clipping_region = state.__clip;
			_transform = state.__transform;

			if (_isActiveInstance()) {
				_applyClip();
				_applyTransform();
			}

		}

		void Graphics::SetTransform(const Transform& transform) {

			_transform = transform;

			if (_isActiveInstance())
				_applyTransform();

		}
		const Transform& Graphics::GetTransform() const {

			return _transform;

		}
		void Graphics::ResetTransform() {

			_transform.Reset();

			if (_isActiveInstance())
				_applyTransform();

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

			_makeThisActiveInstance(false);

			al_set_blender(op, src, dest);

		}
		void Graphics::SetBlendMode(BlendOperation operation, const Color& source, const Color& destination) {

			throw System::NotImplementedException();

		}
		void Graphics::ResetBlendMode() {

			SetBlendMode(BlendOperation::Normal);

		}

		void Graphics::_makeThisActiveInstance(bool writing) const {

			if (writing)
				_canvas->_perform_pre_write_operations();

			if (!_isActiveInstance()) {

				al_set_target_bitmap(System::AllegroAdapter::ToBitmap(*_canvas));

				_applyClip();
				_applyTransform();

				_last_to_draw = this;

			}

		}

		void Graphics::_applyTransform() const {

			al_use_transform((ALLEGRO_TRANSFORM*)&_transform);

		}
		void Graphics::_applyClip() const {

			al_set_clipping_rectangle(_clipping_region.X(), _clipping_region.Y(), _clipping_region.Width(), _clipping_region.Height());

		}
		bool Graphics::_isActiveInstance() const {

			return (_last_to_draw == this);

		}

		const Graphics* Graphics::_last_to_draw = nullptr;

	}

}