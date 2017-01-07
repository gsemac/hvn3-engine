#include "Environment.h"
#include "Exception.h"
#include <allegro5\allegro.h>
#ifdef _WIN64
#define ENVIRONMENT WINDOWS_64BIT
#elif _WIN32
#define ENVIRONMENT WINDOWS_32BIT
#elif __unix || __unix__
#define ENVIRONMENT UNIX
#elif __APPLE__ || __MACH__
#define ENVIRONMENT MAC_OSX
#elif __linux__
#define ENVIRONMENT LINUX
#elif __FreeBSD__
#define ENVIRONMENT FREE_BSD
#else
#define ENVIRONMENT UNKNOWN
#endif
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif
#define MAX_PATH 255 // Not applicable for all platforms

enum OPERATING_SYSTEM {
	UNKNOWN = 0x00,
	WINDOWS_32BIT = 0x01,
	WINDOWS_64BIT = 0x02,
	UNIX = 0x04,
	MAC_OSX = 0x08,
	LINUX = 0x16,
	FREE_BSD = 0x32,
	WINDOWS = WINDOWS_32BIT | WINDOWS_64BIT,
	UNIX_BASED = UNIX | MAC_OSX | LINUX | FREE_BSD
};

// Static methods and member variables

std::string Environment::CurrentDirectory() {

	// Attempt to get the current working directory.
	char* dir = al_get_current_directory();

	if (dir) {
		
		// If the directory is not null, copy the contents of the buffer to a string.
		std::string current_directory = dir;

		// Free the buffer.
		al_free(dir);

		// Return the directory with the executable named trimmed off.
		current_directory = current_directory.substr(0, current_directory.rfind(Environment::PathSeparator()));
		return current_directory;

	}
	else
		// Otherwise, throw an exception.
		throw Exception("Unable to get the current directory.");

}
char Environment::PathSeparator() {

	if (ENVIRONMENT & WINDOWS)
		return '\\';
	else
		return '/';

}

// Helper methods

std::string get_current_directory() {

	// Get the current working directory.
	char buf[MAX_PATH];
	char* cwd = getcwd(buf, MAX_PATH);

	if (cwd) {
		
		// If the directory is not null, copy the contents of the buffer to a string.
		std::string current_directory = cwd;

		// Return the directory with the executable named trimmed off.
		return current_directory.substr(0, current_directory.rfind(Environment::PathSeparator()));

	}
	else
		// Otherwise, throw an exception.
		throw Exception("Unable to get the current directory (getcwd returned null).");

}