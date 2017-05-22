#pragma once
#include "BitFlags.h"
#include <string>
#include <vector>

namespace hvn3 {

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
		friend class Framework;

	public:
		static std::string CurrentDirectory();
		static OperatingSystem OperatingSystem();
		static std::vector<std::string> GetCommandLineArgs();

	private:
		static int argc;
		static char** argv;

	};

}