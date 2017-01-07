#include "Path.h"
#include "Environment.h"

namespace IO {

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

}

