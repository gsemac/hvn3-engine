#include "hvn3/io/IOException.h"

#include "hvn3/utility/StringUtils.h"

namespace hvn3 {
	namespace IO {

		IOException::IOException() : Exception("I/O error occurred.") {}
		FileNotFoundException::FileNotFoundException(const std::string& filename) : IOException(StringUtils::Format("The file \"{0}\" was not found.", filename)) {}

	}
}