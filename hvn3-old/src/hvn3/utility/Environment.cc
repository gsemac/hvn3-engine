#include "hvn3/io/IOException.h"
#include "hvn3/io/Path.h"
#include "hvn3/native/OperatingSystemDefs.h"
#include "hvn3/utility/Environment.h"
#include <allegro5/allegro.h>
#include <algorithm>
#include <thread>

#if defined(HVN3_OS_WINDOWS)
#define INFO_BUFFER_SIZE 32767 // Used when getting machine name/username. 
#include <Windows.h>
#include <psapi.h>
#include <direct.h>
#define getcwd _getcwd
#elif defined(HVN3_OS_UNIX_BASED)
#include <unistd.h>
#include <limits.h>
#include <string.h> 
#endif

#ifndef MAX_PATH
#define MAX_PATH 255 // Not applicable for all platforms
#endif

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
	void Environment::SetCommandLineArgs(int argc, char *argv[]) {

		Environment::argc = argc;
		Environment::argv = argv;

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
	bool strncmp(const char* str_1, const char* str_2, size_t n) {

		size_t i;
		size_t len_1 = strlen(str_1);
		size_t len_2 = strlen(str_2);

		for (i = 0; i < len_1 && i < len_2 && i < n; ++i)
			if (*(str_1 + i) != *(str_2 + i))
				break;

		return i != n;

	}

	// Screen class
	Environment::Screen::Screen(const Rectangle<int>& bounds) :
		_bounds(bounds) {
	}
	const Rectangle<int>& Environment::Screen::Bounds() const {

		return _bounds;

	}
	SizeI Environment::Screen::Resolution() const {

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

	namespace environment {

		bool Is64BitOperatingSystem() {

#if defined(HVN3_OS_X64)

			// 64-bit programs will only run on 64-bit operating systems.
			return true;

#elif defined(HVN3_OS_WINDOWS)

			// A program compiled for a 32-bit platform could still be running under a 64-bit operating system.
			// The check below is described in detail here: https://blogs.msdn.microsoft.com/oldnewthing/20050201-00/?p=36553
			// Note that this may not work for versions of Windows older than Windows XP SP2.
			// #todo Make this check work properly for older versions of Windows.

			BOOL f64 = FALSE;
			return IsWow64Process(GetCurrentProcess(), &f64) && f64;

#else

			// #todo Include check for other operating systems.
			return false;

#endif

		}
		bool Is64BitProcess() {

#ifdef HVN3_OS_X64

			return true;

#else

			return false;

#endif

		}

		std::string MachineName() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];
			DWORD buffer_char_count = INFO_BUFFER_SIZE;

			if (!GetComputerName(info_buffer, &buffer_char_count))
				return "";

			return std::string(info_buffer, buffer_char_count);

#elif defined(HVN3_OS_UNIX_BASED)

			char hostname[HOST_NAME_MAX];
			if (gethostname(hostname, HOST_NAME_MAX) != 0)
				return "";

			return std::string(hostname);

#else

			return "";

#endif

		}
		std::string UserDomainName() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];
			DWORD buffer_size = INFO_BUFFER_SIZE;

			if (!GetComputerNameEx(ComputerNameDnsDomain, info_buffer, &buffer_size))
				return "";

			return std::string(info_buffer, buffer_size);

#else

			return "";

#endif

		}
		std::string UserName() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];
			DWORD buffer_char_count = INFO_BUFFER_SIZE;

			if (!GetUserName(info_buffer, &buffer_char_count))
				return "";

			return std::string(info_buffer, buffer_char_count);

#elif defined(HVN3_OS_UNIX_BASED)

			char username[LOGIN_NAME_MAX];

			if (getlogin_r(username, LOGIN_NAME_MAX) != 0)
				return "";

			return std::string(username);

#else

			return "";

#endif

		}

		std::string SystemDirectory() {

#if defined(HVN3_OS_WINDOWS)

			TCHAR info_buffer[INFO_BUFFER_SIZE];

			if (!GetSystemDirectory(info_buffer, INFO_BUFFER_SIZE))
				return "";

			return std::string(info_buffer);

#else

			return "";

#endif

		}

		std::string NewLine() {

#if defined(HVN3_OS_WINDOWS)

			return "\r\n";

#elif defined(HVN3_OS_MACOS)

			return "\r";

#else

			return "\n";

#endif

		}

		unsigned int ProcessorCount() {

			return std::thread::hardware_concurrency();

		}
		unsigned long SystemPageSize() {

#if defined(HVN3_OS_WINDOWS)

			SYSTEM_INFO system_info;
			GetSystemInfo(&system_info);

			return system_info.dwPageSize;

#elif defined(HVN3_OS_UNIX_BASED)

			return static_cast<unsigned long>(sysconf(_SC_PAGE_SIZE));

#else

			return 0;

#endif

		}
		unsigned long WorkingSet() {

#if defined(HVN3_OS_WINDOWS)

			HANDLE process_handle;
			DWORD process_id = GetCurrentProcessId();
			PROCESS_MEMORY_COUNTERS_EX process_memory_counters;
			unsigned long working_set_size;

			process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);

			if (process_handle == NULL)
				return 0;

			if (GetProcessMemoryInfo(process_handle, (PROCESS_MEMORY_COUNTERS*)&process_memory_counters, sizeof(process_memory_counters)))
				working_set_size = process_memory_counters.WorkingSetSize;

			CloseHandle(process_handle);

			return working_set_size;

#elif defined(HVN3_OS_UNIX_BASED)

			FILE* file = fopen("/proc/self/status", "r");
			int result = -1;
			char line[128];

			while (fgets(line, 128, file) != NULL)
				if (strncmp(line, "VmRSS:", 6) == 0) {

					int len = strlen(line);
					const char* p = line;

					while (*p <'0' || *p > '9')
						p++;

					line[len - 3] = '\0';

					result = atoi(p); // Result will be in KB

					break;

				}

			fclose(file);

			return static_cast<unsigned long>(result * 1024);

#else

			return 0;

#endif

		}

	}

}