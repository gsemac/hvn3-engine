#pragma once

#include "graphics/graphics_base.h"

namespace hvn3::graphics {

	class Graphics :
		public GraphicsBase {

	public:
		void DrawPixel(float x, float y, const Color& color) override;

		void DrawLine(const math::LineF& line, const IPen& pen) override;

		void DrawRectangle(const math::RectangleF& rectangle, const IPen& pen) override;
		void FillRectangle(const math::RectangleF& rectangle, const Color& color) override;

		void DrawRoundedRectangle(const math::RectangleF& rectangle, float radius, const IPen& pen) override;
		void FillRoundedRectangle(const math::RectangleF& rectangle, float radius, const Color& color) override;

		void DrawCircle(float x, float y, float radius, const Color& color, float thickness) override;
		void FillCircle(float x, float y, float radius, const Color& color) override;

		void DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) override;

		void DrawBitmap(float x, float y, const Bitmap& bitmap, float scaleX, float scaleY, float originX, float originY, float angle, const Color& tint) override;

		void Clear(const Color& color) override;
		void Fill(const Color& color) override;

		math::RectangleI Clip() const override;
		void SetClip(int x, int y, int width, int height) override;
		void ResetClip() override;

		class Transform Transform() const override;
		void SetTransform(const class Transform& transform) override;
		void ResetTransform() override;

	private:

	};

}