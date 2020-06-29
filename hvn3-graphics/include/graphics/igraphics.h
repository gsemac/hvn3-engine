#pragma once

#include "graphics/alignment.h"

namespace hvn3::graphics {

	class Color;
	class Font;

	class IGraphics {

		virtual void DrawPixel(float x, float y, const Color& color) = 0;

		virtual void DrawLine(float x, float y, float width, float height, const Color& color, float thickness) = 0;

		virtual void DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) = 0;
		virtual void FillRectangle(float x, float y, float width, float height, const Color& color) = 0;

		virtual void DrawCircle(float x, float y, float radius, const Color& color, float thickness) = 0;
		virtual void FillCircle(float x, float y, float radius, const Color& color) = 0;

		virtual void Clear(const Color& color) = 0;
		virtual void Fill(const Color& color) = 0;

		virtual void DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) = 0;

	};

}