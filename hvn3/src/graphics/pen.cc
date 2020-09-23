#include "graphics/pen.h"

#include "graphics/igraphics.h"

namespace hvn3::graphics {

	Pen::Pen(const Color& color) :
		Pen(color, 1.0f) {
	}
	Pen::Pen(const Color& color, float width) {

		this->color = color;
		this->width = width;

	}

	void Pen::Draw(IGraphics& graphics, float x1, float y1, float x2, float y2) {



	}

}