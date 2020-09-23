#pragma once
#include "hvn3/gui2/WidgetStyle.h"

namespace hvn3 {
	namespace Gui {

		class WidgetStyleSheetReader {
		public:
			static bool ParseBackgroundProperty(const std::string& name, const std::string& value, WidgetStyleProperty<WidgetStyle::WidgetStyleBackgroundProperty>& out);
		private:
			static bool _parseBackgroundColorProperty(const std::string& value, WidgetStyleProperty<WidgetStyle::WidgetStyleBackgroundProperty>& out);
		};

	}
}