#pragma once
#include <string>

namespace hvn3 {
	namespace Xml {

		std::string EscapeXmlString(const std::string& str);
		std::string UnescapeXmlString(const std::string& str);

	}
}