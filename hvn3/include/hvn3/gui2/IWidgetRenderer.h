#pragma once
#include "hvn3/gui2/WidgetStyle.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	namespace Graphics {
		class Graphics;
	}

	namespace Gui {

		class IWidget;
		class WidgetRenderArgs;

		class IWidgetRenderer {

		public:
			virtual ~IWidgetRenderer() = default;

			virtual void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const = 0;
			virtual SizeF MeasureString(const String& string) const = 0;
			virtual const Font& GetWidgetFont(const IWidget* widget) const = 0;

			virtual void AddStyle(const std::string& _class, const WidgetStyle& style) = 0;

		};

	}
}