#pragma once
#include "BitFlags.h"
#include "Size.h"
#include "Rectangle.h"
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
		static OperatingSystem OperatingSystem();
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

}