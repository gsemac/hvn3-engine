#pragma once

namespace hvn3::graphics {

	class IGraphics;

	class IPen {

	public:
		virtual void Draw(IGraphics& graphics, float x1, float y1, float x2, float y2) = 0;

	};

}