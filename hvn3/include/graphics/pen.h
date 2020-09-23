#pragma once

#include "graphics/color.h"
#include "graphics/ipen.h"

namespace hvn3::graphics {

	class Pen :
		public IPen {

	public:
		Pen(const Color& color);
		Pen(const Color& color, float width);

		void Draw(IGraphics& graphics, float x1, float y1, float x2, float y2) override;

	private:
		Color color;
		float width;

	};

}