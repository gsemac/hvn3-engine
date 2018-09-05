#pragma once
#include "hvn3/utility/Version.h"
#include <string>

namespace hvn3 {
	namespace environment {

		enum class Platform {
			Unknown,
			Windows,
			MacOS,
			Linux,
			BSD
		};

		enum class WindowsVersion {
			Unknown,
			Windows95,
			Windows98,
			Windows2000,
			WindowsME,
			WindowsXP,
			WindowsXpProfessional,
			WindowsServer2003,
			WindowsVista,
			WindowsServer2008,
			Windows7,
			WindowsServer2008R2,
			Windows8,
			WindowsServer2012,
			Windows81,
			WindowsServer2012R2,
			Windows10,
			WindowsServer2016
		};

		class PlatformInfo {

		public:
			PlatformInfo(environment::Platform os, const std::string& name, const hvn3::Version& version, const hvn3::Version& service_pack_version, bool is_64_bit);

			// Returns a value representing the platform.
			environment::Platform Platform() const;
			// Returns a string representing the name of the platform/operating system.
			const std::string& Name() const;
			// Returns the version number of the installed service pack.
			const hvn3::Version& ServicePack() const;
			// Returns the version of the platform/operating system.
			const hvn3::Version& Version() const;

			// Returns true for x64 platforms. Returns false otherwise.
			bool Is64Bit() const;
			// Returns true if the platform is Unix-based.
			bool IsUnixBased() const;

		private:
			environment::Platform _os;
			std::string _name;
			hvn3::Version _version;
			hvn3::Version _service_pack;
			bool _is_64_bit;

		};

		// Returns a value representing the current platform.
		constexpr Platform OperatingSystem();
		// Returns an object containing information about the current platform and operating environment.
		PlatformInfo OperatingSystemInfo();
		// Returns a value representing a Windows version given a Windows version number.
		WindowsVersion GetWindowsVersionFromVersionNumber(const Version& version);

	}
}