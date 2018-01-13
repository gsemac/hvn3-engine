#pragma once
#include "hvn3/graphics/Graphics.h"

namespace hvn3 {
	namespace Gui {

		class IWidgetRenderer {

		public:
			virtual ~IWidgetRenderer() = default;

			virtual void DrawWidgetBase(Graphics::Graphics& surface, float x, float y, float width, float height) = 0;

		};

	}
}