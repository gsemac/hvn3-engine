#pragma once
#include "hvn3/utility/BitFlags.h"
#include "hvn3/utility/Size.h"
#include "hvn3/math/Rectangle.h"
#include <string>
#include <vector>

namespace hvn3 {

	namespace System {
		class Framework;
	}

	enum class OperatingSystem {
		Unknown = 0x00,
		Windows32 = 0x01,
		Windows64 = 0x02,
		Unix = 0x04,
		MacOSX = 0x08,
		Linux = 0x16,
		FreeBSD = 0x32,
		Windows = Windows32 | Windows64,
		UnixBased = Unix | MacOSX | Linux | FreeBSD
	};
	ENABLE_BITFLAG_OPERATORS(OperatingSystem);

	class Environment {
		friend class System::Framework;

	public:
		static std::string CurrentDirectory();
		static std::vector<std::string> GetCommandLineArgs();

		class Screen {

		public:			
			const Rectangle<int>& Bounds() const;
			SizeI Resolution() const;

			static Screen PrimaryScreen();
			static Screen VirtualScreen();
			static std::vector<Screen> AllScreens();
			static int ScreenCount();

		private:
			Screen(const Rectangle<int>& bounds);

			Rectangle<int> _bounds;

		};
		
	private:
		static int argc;
		static char** argv;

	};

	namespace environment {

		// Returns true if the program is running on an x64 platform. Returns false otherwise.
		bool Is64BitOperatingSystem();
		// Returns true if the program was compiled for x64 platforms. Returns false otherwise.
		bool Is64BitProcess();

		// Returns the name of the machine that the program is running on.
		std::string MachineName();
		// On Windows, returns the network domain name associated with the current user. On other platforms, returns an empty string.
		std::string UserDomainName();
		// Returns the user name of the user running the program.
		std::string UserName();

		// On Windows, returns the fully-qualified path to the System32 folder on x86 platforms, or the SysWoW64 folder on x64 platforms. On other platforms, returns an empty string.
		std::string SystemDirectory();

		// Returns a string representing the newline character for the current platform.
		std::string NewLine();

		// Returns the number of processors on the current machine.
		unsigned int ProcessorCount();
		// Returns the number of bytes in the operating system's memory page.
		unsigned long SystemPageSize();
		// Returns the number of bytes of physical memory mapped to the current process.
		unsigned long WorkingSet();

	}

}