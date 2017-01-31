#pragma once
#include <string>
#include "Color.h"
#include "Geometry.h"
#include "Font.h"
#include "UTF8String.h"
#include "View.h"
#include "Transform.h"
#include "Bitmap.h"

namespace Drawing {

	class Graphics {

	public:
		Graphics(Bitmap& surface);

		void DrawRectangle(const Rectangle& rect, const Color& color, float thickness);
		void DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness);
		void DrawFilledRectangle(const Rectangle& rect, const Color& color);
		void DrawFilledRectangle(float x, float y, float width, float height, const Color& color);

		void DrawRoundRectangle(const Rectangle& rect, const Color& color, float radius, float thickness);
		void DrawRoundRectangle(float x, float y, float width, float height, const Color& color, float radius, float thickness);
		void DrawFilledRoundRectangle(const Rectangle& rect, const Color& color, float radius);
		void DrawFilledRoundRectangle(float x, float y, float width, float height, const Color& color, float radius);

		void DrawLine(const Line& line);
		void DrawLine(const Line& line, const Color& color, float thickness);
		void DrawLine(const Point& p1, const Point& p2, const Color& color, float thickness);
		void DrawLine(float x1, float y1, float x2, float y2, const Color& color, float thickness);

		void DrawPoint(const Point& point, const Color& color);
		void DrawPoint(float x, float y, const Color& color);

		void DrawCircle(const Point& point, float radius, const Color& color, float thickness);
		void DrawCircle(float x, float y, float radius, const Color& color, float thickness);
		void DrawFilledCircle(const Point& point, float radius, const Color& color);
		void DrawFilledCircle(float x, float y, float radius, const Color& color);

		void Clear(const Color& color);

		void DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment align = Alignment::Left);
		void DrawText(float x, float y, const std::string& text, const Font& font, const Color& color);
		void DrawText(float x, float y, Utf8String& text, const Font& font, const Color& color);

		void DrawSprite(const Sprite& sprite, int subimage, float x, float y);
		void DrawSprite(const Sprite& sprite, int subimage, float x, float y, const Color& blend, float xscale, float yscale, float angle);

		void DrawBitmap(const Bitmap& bitmap, float x, float y);
		void DrawBitmap(const Bitmap& bitmap, float x, float y, float xscale, float yscale);
		void DrawBitmap(const Bitmap& bitmap, const Rectangle& region, float x, float y);

		void SetClip(const Rectangle& rect);
		void SetClip(int x, int y, int width, int height);
		Rectangle Clip() const;
		void ResetClip();

		void SetTransform(const Transform& transform);
		const Transform& GetTransform() const;
		void ResetTransform();

		void HoldBitmapDrawing(bool hold);

	private:
		Bitmap __surface;
		Transform __transform;
		Rectangle __clipping_region;

		// Called at the beginning of every drawing function to set the drawing target and its and tranforms/clipping region.
		void PrepareDrawingSurface();
		void ApplyTransform();
		void ApplyClip();
		bool IsActiveSurface() const;

	};

}