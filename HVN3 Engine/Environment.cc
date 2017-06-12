#include "Environment.h"
#include "io/Path.h"
#include "Exception.h"
#include <allegro5\allegro.h>
#include <algorithm>
#ifdef _WIN64
#define ENVIRONMENT hvn3::OperatingSystem::Windows64
#elif _WIN32
#define ENVIRONMENT hvn3::OperatingSystem::Windows32
#elif __unix || __unix__
#define ENVIRONMENT hvn3::OperatingSystem::WindowsUnix
#elif __APPLE__ || __MACH__
#define ENVIRONMENT hvn3::OperatingSystem::MacOSX
#elif __linux__
#define ENVIRONMENT hvn3::OperatingSystem::Linux
#elif __FreeBSD__
#define ENVIRONMENT hvn3::OperatingSystem::FreeBSD
#else
#define ENVIRONMENT hvn3::OperatingSystem::Unknown
#endif
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif
#define MAX_PATH 255 // Not applicable for all platforms

namespace hvn3 {

	// Static methods and member variables

	std::string Environment::CurrentDirectory() {

		// Attempt to get the current working directory.
		char* dir = al_get_current_directory();

		if (dir) {

			// If the directory is not null, copy the contents of the buffer to a string.
			std::string current_directory = dir;

			// Free the buffer.
			al_free(dir);

			// Return the directory.
			return current_directory;

		}
		else
			// Otherwise, throw an exception.
			throw IO::IOException("Failed to get the current working directory.");

	}
	hvn3::OperatingSystem Environment::OperatingSystem() {

		return ENVIRONMENT;

	}
	std::vector<std::string> Environment::GetCommandLineArgs() {

		// Use a static declaration so it only needs to be built once.
		static std::vector<std::string> args;

		// Return the arguments if the collection has already been built.
		if (args.size() > 0)
			return args;

		// Otherwise, build the collection.
		for (int i = 0; i < argc; ++i)
			args.push_back(std::string(argv[i]));

		// Return the result.
		return args;

	}

	int Environment::argc = 0;
	char** Environment::argv = nullptr;

	// Helper methods

	std::string get_current_directory() {

		// Get the current working directory.
		char buf[MAX_PATH];
		char* cwd = getcwd(buf, MAX_PATH);

		if (cwd)
			// If the directory is not null, copy the contents of the buffer to a string and return the result.
			return std::string(cwd);
		else
			// Otherwise, throw an exception.
			throw Exception("Unable to get the current directory (getcwd returned null).");

	}

	// Screen class
	Environment::Screen::Screen(const Rectangle<int>& bounds) :
		_bounds(bounds) {
	}
	const Rectangle<int>& Environment::Screen::Bounds() const {

		return _bounds;

	}
	const Size<int>& Environment::Screen::Resolution() const {

		return _bounds.Size();

	}
	Environment::Screen Environment::Screen::PrimaryScreen() {

		// The primary screen is the one at (0, 0).
		// https://www.allegro.cc/manual/5/allegro_monitor_info

		int count = ScreenCount();
		ALLEGRO_MONITOR_INFO info;

		for (int adapter = 0; adapter < count; ++adapter)
			if (al_get_monitor_info(adapter, &info))
				if (info.x1 == 0 && info.y1 == 0)
					return Screen(Rectangle<int>(info.x1, info.y1, info.x2 - info.x1, info.y2 - info.y1));

		throw Exception("Could not find primary display.");

	}
	Environment::Screen Environment::Screen::VirtualScreen() {

		int count = ScreenCount();
		ALLEGRO_MONITOR_INFO info;
		int x1 = 0, y1 = 0;
		int x2 = 0, y2 = 0;

		for (int adapter = 0; adapter < count; ++adapter)
			if (al_get_monitor_info(adapter, &info)) {
				x1 = (std::min)(x1, info.x1);
				y1 = (std::min)(y1, info.y1);
				x2 = (std::max)(x2, info.x2);
				y2 = (std::max)(y2, info.y2);
			}

		return Screen(Rectangle<int>(x1, y1, x2 - x1, y2 - y1));

	}
	std::vector<Environment::Screen> Environment::Screen::AllScreens() {

		int count = ScreenCount();
		std::vector<Screen> screens;
		ALLEGRO_MONITOR_INFO info;

		for (int adapter = 0; adapter < count; ++adapter)
			if (al_get_monitor_info(adapter, &info))
				screens.push_back(Screen(Rectangle<int>(info.x1, info.y1, info.x2 - info.x1, info.y2 - info.y1)));

		return screens;

	}
	int Environment::Screen::ScreenCount() {

		return al_get_num_video_adapters();

	}

}