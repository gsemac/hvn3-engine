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
#if ENVIRONMENT & WINDOWS
#include <direct.h>
#define getcwd _getcwd
elif
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

	char* dir = al_get_current_directory();
	if (dir) {
		std::string current_directory = dir;
		current_directory = current_directory.substr(0, current_directory.rfind(Environment::PathSeparator()));
		al_free(dir);
	}

}
char Environment::PathSeparator() {

	if (ENVIRONMENT & WINDOWS)
		return '\\';
	else
		return '/';

}

// Helper methods

std::string get_current_directory() {

	char buf[MAX_PATH];
	char* cwd = getcwd(buf, MAX_PATH);
	if (cwd) {

		std::string current_directory = cwd;
		return current_directory.substr(0, current_directory.rfind(Environment::PathSeparator()));

	}
	else
		throw Exception("getcwd returned null.");

}