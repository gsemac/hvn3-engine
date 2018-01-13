#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class WidgetRenderer : public IWidgetRenderer {

		public:
			virtual void DrawWidgetBase(Graphics::Graphics& surface, float x, float y, float width, float height) override;

		private:

		};

	}
}