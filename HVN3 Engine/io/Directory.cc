#include "Directory.h"
#include "Path.h"
#include "Environment.h"
#include "StringUtils.h"
#include "Exception.h"
#include <allegro5/allegro.h>

namespace hvn3 {

	namespace IO {

		// Static methods and member variables

		const std::string Directory::GetCurrentDirectory() {

			// Return the current working directory.
			return Environment::CurrentDirectory();

		}
		void Directory::SetCurrentDirectory(const std::string path) {

			// Set the new working directory.
			if (!al_change_directory(StringUtils::RTrim(path, { IO::Path::DirectorySeparatorChar() }).c_str()))
				throw IOException("Failed to set the working directory.");

		}

	}

}