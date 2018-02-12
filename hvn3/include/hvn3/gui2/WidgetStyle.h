#pragma once
#include "hvn3/fonts/Font.h"
#include "hvn3/graphics/Color.h"

namespace hvn3 {
	namespace Gui {

		class WidgetStyle {

		public:
			WidgetStyle();

			Color foreColor;
			Color backColor;
			Font font;

		};

	}
}