#include "hvn3/exceptions/Exception.h"

namespace hvn3 {

	ArgumentException::ArgumentException() : ExceptionBase("An invalid argument was passed.") {}
	InvalidOperationException::InvalidOperationException() : Exception("") {}
	NotSupportedException::NotSupportedException() : Exception("Specified method is not supported.") {}
	NotImplementedException::NotImplementedException() : Exception("Function not yet implemented.") {}
	FrameworkException::FrameworkException() : Exception("An error was thrown by the engine framework.") {}
	KeyNotFoundException::KeyNotFoundException() : Exception("The given key could not be found in the collection.") {}
	NullPointerException::NullPointerException() : Exception("An attempt was made to read a null pointer.") {}

}