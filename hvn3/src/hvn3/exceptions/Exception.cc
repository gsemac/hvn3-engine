#include "hvn3/exceptions/Exception.h"

namespace hvn3 {
	namespace System {

		Exception::Exception() : std::runtime_error("") {}
		Exception::Exception(const char* message) : std::runtime_error(message) {}
		Exception::Exception(const std::string& message) : std::runtime_error(message.c_str()) {}
		const char* Exception::what() const throw() {

			return std::runtime_error::what();

		}
		std::string Exception::Message() const {

			return std::string(what());

		}

		InvalidOperationException::InvalidOperationException() : Exception("") {}
		NotSupportedException::NotSupportedException() : Exception("Specified method is not supported.") {}
		NotImplementedException::NotImplementedException() : Exception("Function not yet implemented.") {}
		FrameworkException::FrameworkException() : Exception("An error was thrown by the engine framework.") {}
		KeyNotFoundException::KeyNotFoundException() : Exception("The given key could not be found in the collection.") {}
		NullPointerException::NullPointerException() : Exception("An attempt was made to read a null pointer.") {}

		IO::IOException::IOException() : Exception("I/O error occurred.") {}
		IO::FileNotFoundException::FileNotFoundException() : IOException("Unable to find the specified file.") {}

	}
}