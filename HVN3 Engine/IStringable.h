#pragma once
#include <string>

namespace hvn3 {

	struct IStringable {
		// Returns the text representation of the value of the current object.
		virtual std::string ToString() = 0;
	};

}