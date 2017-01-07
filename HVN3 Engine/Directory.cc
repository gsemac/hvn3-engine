#include "Directory.h"
#include "Exception.h"
#include <cstdlib>
#include <allegro5\allegro.h>
#define MAX_PATH 255 // Not applicable for all platforms
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define PATH_SEPARATOR '\\'
#elif
#include <unistd.h>
#define PATH_SEPARATOR '/'
#endif

namespace IO {

	// Static methods and member variables

	std::string Directory::__current_directory;
	const std::string Directory::GetCurrentDirectory() {

		if (__current_directory.empty()) {

			// If the current directory is empty (hasn't been set yet), intiailize it.
			char* dir = al_get_current_directory();
			if (dir) {
				__current_directory = dir;
				__current_directory = __current_directory.substr(0, __current_directory.rfind(PATH_SEPARATOR));
				al_free(dir);
			}

		}

		// Return the current directory.
		return __current_directory;

	}
	void Directory::SetCurrentDirectory(const std::string path) {

		__current_directory = path;

	}

	// Helper methods

	std::string get_current_directory() {

		char buf[MAX_PATH];
		char* cwd = getcwd(buf, MAX_PATH);
		if (cwd) {

			std::string current_directory = cwd;
			return current_directory.substr(0, current_directory.rfind(PATH_SEPARATOR));

		}
		else
			throw Exception("getcwd returned null.");

	}

}