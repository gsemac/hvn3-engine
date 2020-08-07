#pragma once

#include "graphics/alignment.h"
#include "graphics/transform.h"
#include "math/line.h"
#include "math/rectangle.h"

namespace hvn3::graphics {

	class Bitmap;
	class Color;
	class Font;
	class IPen;

	class IGraphics {

	public:
		//virtual void DrawPixel(float x, float y, const Color& color) = 0;

		//virtual void DrawLine(float startX, float startY, float endX, float endY, const Color& color, float thickness) = 0;
		//virtual void DrawLine(float startX, float startY, float endX, float endY, const IPen& pen) = 0;
		//virtual void DrawLine(const math::LineF& line, const Color& color, float thickness) = 0;
		//virtual void DrawLine(const math::LineF& line, const IPen& pen) = 0;

		//virtual void DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) = 0;
		//virtual void DrawRectangle(float x, float y, float width, float height, const IPen& pen) = 0;
		//virtual void DrawRectangle(const math::RectangleF& rectangle, const Color& color, float thickness) = 0;
		//virtual void DrawRectangle(const math::RectangleF& rectangle, const IPen& pen) = 0;

		//virtual void FillRectangle(float x, float y, float width, float height, const Color& color) = 0;
		//virtual void FillRectangle(const math::RectangleF& rectangle, const Color& color) = 0;

		//virtual void DrawRoundedRectangle(float x, float y, float width, float height, float radius, const Color& color, float thickness) = 0;
		//virtual void DrawRoundedRectangle(float x, float y, float width, float height, float radius, const IPen& pen) = 0;
		//virtual void DrawRoundedRectangle(const math::RectangleF& rectangle, float radius, const Color& color, float thickness) = 0;
		//virtual void DrawRoundedRectangle(const math::RectangleF& rectangle, float radius, const IPen& pen) = 0;

		//virtual void FillRoundedRectangle(float x, float y, float width, float height, float radius, const Color& color) = 0;
		//virtual void FillRoundedRectangle(const math::RectangleF& rectangle, float radius, const Color& color) = 0;

		//virtual void DrawCircle(float x, float y, float radius, const Color& color, float thickness) = 0;

		//virtual void FillCircle(float x, float y, float radius, const Color& color) = 0;

		//virtual void DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) = 0;

		//virtual void DrawBitmap(float x, float y, const Bitmap& bitmap, float scaleX, float scaleY, float originX, float originY, float angle, const Color& tint) = 0;

		virtual void Clear(const Color& color) = 0;
		//virtual void Fill(const Color& color) = 0;

		//virtual math::RectangleI Clip() const = 0;
		//virtual void SetClip(int x, int y, int width, int height) = 0;
		//virtual void ResetClip() = 0;

		//virtual Transform Transform() const = 0;
		//virtual void SetTransform(const class Transform& transform) = 0;
		//virtual void ResetTransform() = 0;

	};

}