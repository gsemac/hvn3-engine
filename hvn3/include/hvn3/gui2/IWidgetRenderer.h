#pragma once
#include "hvn3/gui2/GuiTypeDefs.h"

namespace hvn3 {

	namespace Graphics {
		class Graphics;
	}

	namespace Gui {

		class IButton;

		class IWidgetRenderer {

		public:
			virtual ~IWidgetRenderer() = default;

			virtual void ApplyStyleToWidget(IWidget& widget) = 0;
			virtual void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const = 0;

		};

	}
}