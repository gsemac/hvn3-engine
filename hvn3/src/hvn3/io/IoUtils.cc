#include "hvn3/exceptions/Exception.h"
#include "hvn3/io/IoUtils.h"
#include <fstream>
#include <sstream>

namespace hvn3 {
	namespace IO {

		std::string FileToByteString(const std::string& path) {

			std::ifstream fstream(path, std::ios::in | std::ios::binary);
			std::stringstream byte_string;

			if (!fstream.is_open())
				throw System::IO::FileNotFoundException();

			unsigned char next_byte;
			while (fstream >> next_byte) {
				byte_string << "\\x";
				byte_string << std::hex << (int)next_byte;
			}

			return byte_string.str();

		}

	}
}