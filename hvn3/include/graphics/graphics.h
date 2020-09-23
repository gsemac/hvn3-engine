#pragma once

#include "graphics/bitmap.h"
#include "graphics/graphics_base.h"

#include <stack>

namespace hvn3::graphics {

	class Graphics :
		public GraphicsBase {

	public:
		Graphics();
		Graphics(const Bitmap& canvas);

		~Graphics();

		//void DrawPixel(float x, float y, const Color& color) override;
		//void DrawLine(const math::LineF& line, const IPen& pen) override;
		void DrawRectangle(const math::RectangleF& rectangle, const Color& color, float thickness) override;
		//void DrawRectangle(const math::RectangleF& rectangle, const IPen& pen) override;
		//void FillRectangle(const math::RectangleF& rectangle, const Color& color) override;
		//void DrawRoundedRectangle(const math::RectangleF& rectangle, float radius, const IPen& pen) override;
		//void FillRoundedRectangle(const math::RectangleF& rectangle, float radius, const Color& color) override;
		//void DrawCircle(float x, float y, float radius, const Color& color, float thickness) override;
		//void FillCircle(float x, float y, float radius, const Color& color) override;
		//void DrawText(float x, float y, const char* text, const Font& font, const Color& color, Alignment alignment) override;
		//void DrawBitmap(float x, float y, const Bitmap& bitmap, float scaleX, float scaleY, float originX, float originY, float angle, const Color& tint) override;

		void Clear(const Color& color) override;
		//void Fill(const Color& color) override;

		//class Transform Transform() const override;
		//void SetTransform(const class Transform& transform) override;
		//void ResetTransform() override;

		math::RectangleI GetClip() const override;
		void PushClip(const math::RectangleI& clip) override;
		void PopClip() override;
		void ClearClip() override;

		class Transform GetTransform() const override;
		void PushTransform(const class Transform& transform) override;
		void PopTransform() override;
		void ClearTransform() override;

		explicit operator bool() const;

	private:
		Bitmap canvas;
		std::stack<math::RectangleI> clippingRegions;
		std::stack<class Transform> transforms;

		static const Graphics* lastToDraw;

		void PrepareCanvas() const;
		void PrepareCanvas(bool isPainting);
		bool IsCanvasReady() const;

	};

}