#pragma once

#include "graphics/igraphics.h"

namespace hvn3::graphics {

	class GraphicsBase :
		public IGraphics {

	public:
		void DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) override;

		void PushClip(int x, int y, int width, int height) override;

	};

}