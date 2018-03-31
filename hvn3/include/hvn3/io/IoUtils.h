#pragma once
#include <string>

namespace hvn3 {
	namespace IO {

		// Converts a file into a string of bytes, and returns the result.
		std::string FileToByteString(const std::string& path);

	}
}