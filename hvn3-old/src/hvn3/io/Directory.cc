#include "hvn3/io/IOException.h"
#include "hvn3/io/Directory.h"
#include "hvn3/io/Path.h"
#include "hvn3/native/OperatingSystemDefs.h"
#include "hvn3/utility/Environment.h"
#include "hvn3/utility/StringUtils.h"

#include <cassert>
#include <cstdlib>
#include <string>

#include <allegro5/allegro.h>

#if defined(HVN3_OS_WINDOWS)
#include <Windows.h>
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#endif

namespace hvn3 {

	namespace IO {

		// Static methods and member variables

		const std::string Directory::GetCurrentDirectory() {

			// Return the current working directory.
			return Environment::CurrentDirectory();

		}
		const std::string Directory::GetTemporaryDirectory() {

			std::string temp_path_str;

#if defined(HVN3_OS_WINDOWS)

			char temp_path_arr[MAX_PATH];

			if (GetTempPathA(MAX_PATH, temp_path_arr))
				temp_path_str = temp_path_arr;

#elif defined(HVN3_OS_UNIX_BASED)

			const char* temp_path_arr = getenv("TMPDIR");

			if (temp_path_arr == 0)
				temp_path_arr = "/tmp/";

			temp_path_str = temp_path_arr;

#endif	

			assert(temp_path_str.size() > 0);

			if (temp_path_str.back() != Path::DirectorySeparatorChar())
				temp_path_str.push_back(Path::DirectorySeparatorChar());

			return temp_path_str;

		}
		void Directory::SetCurrentDirectory(const std::string path) {

			// Set the new working directory.
			if (!al_change_directory(StringUtils::RTrim(path, { IO::Path::DirectorySeparatorChar() }).c_str()))
				throw IO::IOException("Failed to set the working directory.");

		}

	}

}