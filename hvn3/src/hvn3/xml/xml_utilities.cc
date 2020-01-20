#include "hvn3/utility/StringUtils.h"
#include "hvn3/xml/xml_utilities.h"

namespace hvn3 {
	namespace xml {

		std::string EscapeXmlString(const std::string& str) {

			std::string buf;
			buf.reserve(str.length());

			for (auto i = str.begin(); i != str.end(); ++i) {

				switch (*i) {

				case '"':
					buf += "&quot;";
					break;

				case '\'':
					buf += "&apos;";
					break;

				case '<':
					buf += "&lt;";
					break;

				case '>':
					buf += "&gt;";
					break;

				case '&':
					buf += "&amp;";
					break;

				default:
					buf.push_back(*i);
					break;

				}

			}

			return buf;

		}
		std::string UnescapeXmlString(const std::string& str) {

			std::string buf;
			buf.reserve(str.length());

			for (size_t i = 0; i < str.size(); ++i) {

				if (str[i] == '&') {

					if (StringUtils::NextSubstringEquals(str, i + 1, "quot;")) {
						buf.push_back('"');
						i += 4;
					}
					else if (StringUtils::NextSubstringEquals(str, i + 1, "apos;")) {
						buf.push_back('\'');
						i += 4;
					}
					else if (StringUtils::NextSubstringEquals(str, i + 1, "lt;")) {
						buf.push_back('<');
						i += 2;
					}
					else if (StringUtils::NextSubstringEquals(str, i + 1, "gt;")) {
						buf.push_back('>');
						i += 2;
					}
					else { // &amp;
						buf.push_back('&');
						i += 3;
					}

					continue;

				}

				buf.push_back(str[i]);

			}

			return buf;

		}

	}
}