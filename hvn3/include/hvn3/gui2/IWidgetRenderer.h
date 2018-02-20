#pragma once

namespace hvn3 {

	namespace Graphics {
		class Graphics;
	}

	namespace Gui {

		class IWidget;
		class WidgetRendererRenderArgs;

		class IWidgetRenderer {

		public:
			virtual ~IWidgetRenderer() = default;

			virtual void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget, WidgetRendererRenderArgs& args) const = 0;

		};

	}
}