#include "Directory.h"
#include "Environment.h"

namespace IO {

	// Static methods and member variables

	std::string Directory::__current_directory;

	const std::string Directory::GetCurrentDirectory() {

		// If the current directory hasn't yet been set, get the current directory.
		if (__current_directory.empty())
			__current_directory = Environment::CurrentDirectory();

		// Return the current directory.
		return __current_directory;

	}
	void Directory::SetCurrentDirectory(const std::string path) {

		__current_directory = path;

	}

}