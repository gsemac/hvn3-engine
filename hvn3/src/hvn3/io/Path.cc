#include "hvn3/io/Path.h"
#include "hvn3/utility/Environment.h"
#include "hvn3/utility/StringUtils.h"
#include <cctype>

namespace hvn3 {

	namespace IO {

		std::string Path::Combine(const std::string& path1, const std::string& path2) {

			// If the second path has a root, just return the second path.
			if (IsPathRooted(path2))
				return path2;

			// Begin the new path with the first path.
			std::string new_path = path1;

			// Does it already end with a directory separator? If not, add one.
			if (new_path.length() > 0 && new_path[-1 + new_path.length()] != DirectorySeparatorChar())
				new_path += DirectorySeparatorChar();

			// Append the second path.
			new_path += path2;

			// Return the result.
			return new_path;

		}
		std::string Path::Combine(const std::string& path1, const std::string& path2, const std::string& path3) {

			return Combine(Combine(path1, path2), path3);

		}
		std::string Path::Combine(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4) {

			return Combine(Combine(Combine(path1, path2), path3), path4);

		}
		bool Path::IsPathRooted(const std::string& path) {

			// If the path is empty, return false.
			if (path.length() <= 0)
				return false;

			// If the path begins with a directory separator, return true.
			if (path[0] == DirectorySeparatorChar())
				return true;

			// If the path begins with a letter followed by a colon, return true.
			if (path.length() >= 2 && isalpha(path[0]) && path[1] == ':')
				return true;

			// None of the conditions matched, so it must be relative.
			return false;

		}
		char Path::DirectorySeparatorChar() {

			if (HasFlag(Environment::OperatingSystem(), OperatingSystem::Windows))
				return '\\';
			else
				return '/';

		}
		char Path::AltDirectorySeparatorChar() {

			if (HasFlag(Environment::OperatingSystem(), OperatingSystem::Windows))
				return '/';
			else
				return '\\';

		}
		char Path::VolumeSeparatorChar() {

			if (HasFlag(Environment::OperatingSystem(), OperatingSystem::Windows) ||
				HasFlag(Environment::OperatingSystem(), OperatingSystem::MacOSX))
				return ':';
			else
				return '/';

		}
		std::string Path::GetExtension(const std::string& path) {

			if (path.length() <= 0)
				return "";

			for (size_t i = path.length() - 1; i > 0; --i) {

				if (path[i] == DirectorySeparatorChar() || path[i] == AltDirectorySeparatorChar())
					break;

				if (path[i] == '.')
					return path.substr(i, path.length() - i);

			}

			return "";

		}

	}

}