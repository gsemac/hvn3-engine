#include "Environment.h"
#include "Path.h"
#include "Exception.h"
#include <allegro5\allegro.h>
#ifdef _WIN64
#define ENVIRONMENT OperatingSystem::Windows64
#elif _WIN32
#define ENVIRONMENT OperatingSystem::Windows32
#elif __unix || __unix__
#define ENVIRONMENT OperatingSystem::WindowsUnix
#elif __APPLE__ || __MACH__
#define ENVIRONMENT OperatingSystem::MacOSX
#elif __linux__
#define ENVIRONMENT OperatingSystem::Linux
#elif __FreeBSD__
#define ENVIRONMENT OperatingSystem::FreeBSD
#else
#define ENVIRONMENT OperatingSystem::Unknown
#endif
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif
#define MAX_PATH 255 // Not applicable for all platforms

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
		throw Exception("Unable to get the current directory.");

}
::OperatingSystem Environment::OperatingSystem() {

	return ENVIRONMENT;

}

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