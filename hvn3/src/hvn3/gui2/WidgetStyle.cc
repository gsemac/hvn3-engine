#include "hvn3/exceptions/Exception.h"
#include "hvn3/gui2/WidgetStyle.h"
#include <cctype>

#define COLOR_NAME_MIN_LENGTH 3
#define COLOR_INVALID hvn3::Color()

namespace hvn3 {
	namespace Gui {

		static Color readColor(const std::string& value) {
			if (value.length() <= 3) // Shortest possible color would 3 chars, e.g. "red".
				return COLOR_INVALID;
			try {
				return Color::FromHex(value);
			}
			catch (const System::ArgumentException& ex) {
				return COLOR_INVALID;
			}
		}



		WidgetStyle::WidgetStyle() {}

		WidgetStyle::WidgetStyleBackgroundProperty::WidgetStyleBackgroundProperty() :
			setColors{ 0, 0, 0, 0 } {
			setColors[TOP] = true;
		}
		WidgetStyle::WidgetStyleBackgroundProperty::WidgetStyleBackgroundProperty(const std::string& str) :
			WidgetStyleBackgroundProperty() {
			if (str.length() < COLOR_NAME_MIN_LENGTH)
				return;
			if (str[0] == '#') {
				top = readColor(str);
				setColors[TOP] = true;
			}
			else if (StringUtils::StartsWith(str, "linear-gradient")) {

			}

		}




		std::string WidgetStyle::GetPropertyString(const std::string& name) {
			auto it = _properties.find(name);
			if (it == _properties.end())
				return "";
			return it->second;
		}
		void WidgetStyle::SetProperty(const std::string& name, const std::string& value) {
			_properties[name] = value;
		}


		bool WidgetStyle::_isInherit(const std::string& value) {
			return value == "inherit";
		}

	}
}