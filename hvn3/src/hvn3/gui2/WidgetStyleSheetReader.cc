#include "hvn3/exceptions/Exception.h"
#include "hvn3/gui2/WidgetStyleSheetReader.h"

namespace hvn3 {
	namespace Gui {

		bool WidgetStyleSheetReader::ParseBackgroundProperty(const std::string& name, const std::string& value, WidgetStyleProperty<WidgetStyle::WidgetStyleBackgroundProperty>& out) {
			if (StringUtils::IEquals(name, "background-color"))
				return _parseBackgroundColorProperty(value, out);
			// The property name was not recognized.
			return false;
		}



		bool WidgetStyleSheetReader::_parseBackgroundColorProperty(const std::string& value, WidgetStyleProperty<WidgetStyle::WidgetStyleBackgroundProperty>& out) {

			if (value.length() <= 0)
				return false;

			if (StringUtils::IEquals(value, "inherit")) { // Value is inherited.
				out.inherit = true;
				return true;
			}
			else if (value[0] == '#') { // Value provided as single hex color.
				try {
					out.value.top = Color::FromHex(value);
					out.value.setColors[out.value.TOP] = true;
					return true;
				}
				catch (const System::ArgumentException& ex) {
					return false;
				}
			}

			std::string word;
			size_t index = 0;

			StringUtils::NextWord(value, word, index);

			if (StringUtils::IEquals(word, "linear-gradient")) { // Value provided as a linear gradient.
				try {
					// Only two colors are supported at the moment.
					StringUtils::NextWord(value, word, index);
					out.value.top = Color::FromHex(word);
					StringUtils::NextWord(value, word, index);
					out.value.bottom = Color::FromHex(word);
					out.value.setColors[out.value.TOP] = true;
					out.value.setColors[out.value.BOTTOM] = true;
				}
				catch (const System::ArgumentException& ex) {
					return false;
				}
			}

		}

	}
}