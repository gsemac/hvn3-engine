#pragma once
#include <string>
#include "Color.h"
#include "Geometry.h"
#include "Font.h"
#include "UTF8String.h"
#include "View.h"
#include "Transform.h"
#include "Bitmap.h"
#include "GraphicsState.h"

namespace hvn3 {

	namespace Drawing {

		enum class BlendMode {
			Normal,
			Add,
			Subtract,
			Max,
			DestinationMinusSource,
			SourceMinusDestination,
			Zero,
			One,
			Alpha,
			InverseAlpha
		};

		class Graphics {

		public:
			Graphics(Bitmap& surface);
			~Graphics();

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

			void DrawText(float x, float y, const char* text, const Font* font, const Color& color, Alignment alignment = Alignment::Left);
			void DrawText(float x, float y, const std::string& text, const Font* font, const Color& color);
			void DrawText(float x, float y, const String& text, const Font* font, const Color& color, Alignment alignment = Alignment::Left);

			void DrawSprite(float x, float y, const Sprite& sprite, int subimage);
			void DrawSprite(float x, float y, const Sprite& sprite, int subimage, float xscale, float yscale, float angle, const Color& blend);

			void DrawBitmap(float x, float y, const Bitmap* bitmap);
			void DrawBitmap(float x, float y, const Bitmap* bitmap, float xscale, float yscale);
			void DrawBitmap(float x, float y, const Bitmap* bitmap, float xscale, float yscale, const Point& origin, float angle);
			void DrawBitmap(float x, float y, const Bitmap* bitmap, const Rectangle& region);
			void DrawBitmap(float x, float y, const Bitmap* bitmap, const Color& tint);

			void SetClip(const Rectangle& rect);
			void SetClip(int x, int y, int width, int height);
			Rectangle Clip() const;
			void ResetClip();

			void SetTransform(const Transform& transform);
			const Transform& GetTransform() const;
			void ResetTransform();

			void HoldBitmapDrawing(bool hold);

			GraphicsState Save() const;
			void Restore(const GraphicsState& state);

		private:
			Bitmap _surface;
			Transform _transform;
			Rectangle _clipping_region;
			bool _state_changed;
			static Graphics* _last_to_draw;

			// Called at the beginning of every drawing function to set the drawing target and its and tranforms/clipping region.
			void PrepareDrawingSurface();
			void ApplyTransform();
			void ApplyClip();
			bool IsActiveSurface() const;

			int GetAllegroFlags(Alignment value) const;

		};

	}

}