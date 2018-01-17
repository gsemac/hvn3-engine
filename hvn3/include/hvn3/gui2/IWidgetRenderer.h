#pragma once
#include "hvn3/graphics/Graphics.h"
#include "hvn3/gui2/GuiTypeDefs.h"

namespace hvn3 {
	namespace Gui {

		class IButton;

		class IWidgetRenderer {

		public:
			virtual ~IWidgetRenderer() = default;

			virtual void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const = 0;

		};

	}
}