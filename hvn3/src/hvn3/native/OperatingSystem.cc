#include "hvn3/native/OperatingSystem.h"
#include "hvn3/native/OperatingSystemDefs.h"
#include "hvn3/utility/Environment.h"

#if defined(HVN3_OS_WINDOWS)
#include <Windows.h>
#pragma warning(disable: 4996)
#elif defined(HVN3_OS_UNIX_BASED)
#include <sys/utsname.h>
#endif

#define UNKNOWN_OS_NAME "Unknown"

namespace hvn3 {
	namespace environment {

		Version getOsVersion() {

#ifdef HVN3_OS_WINDOWS

			OSVERSIONINFO ver;
			ver.dwOSVersionInfoSize = sizeof(ver);

			if (GetVersionEx(&ver) == 0)
				return Version();

			return Version(ver.dwMajorVersion, ver.dwMinorVersion, ver.dwBuildNumber, 0);

#elif defined(HVN3_OS_UNIX_BASED)

			return Version();

#else

			return Version();

#endif

		}
		Version getServicePackVersion() {

#if defined(HVN3_OS_WINDOWS)

			OSVERSIONINFOEX ver;
			ver.dwOSVersionInfoSize = sizeof(ver);

			if (GetVersionEx((OSVERSIONINFO*)&ver) == 0)
				return Version();

			return Version(ver.wServicePackMajor, ver.wServicePackMinor);

#else

			return Version();

#endif

		}
		std::string getOsName() {

#if defined(HVN3_OS_WINDOWS)

			OSVERSIONINFOEX ver;
			ver.dwOSVersionInfoSize = sizeof(ver);

			if (GetVersionEx((OSVERSIONINFO*)&ver) == 0)
				return UNKNOWN_OS_NAME;

			// Version info table:
			// https://msdn.microsoft.com/en-us/library/windows/desktop/ms724834%28v=vs.85%29.aspx

			switch (ver.dwMajorVersion) {

			case 5:
				if (ver.dwMinorVersion == 0)
					return "Windows 2000";
				else if (ver.dwMinorVersion == 1)
					return "Windows XP";
				else if (ver.dwMinorVersion == 2)
					return GetSystemMetrics(SM_SERVERR2) == 0 ? "Windows Server 2003" : "Windows Server 2003 R2";
				break;

			case 6:
				if (ver.dwMinorVersion == 0)
					return ver.wProductType == VER_NT_WORKSTATION ? "Windows Vista" : "Windows Server 2008";
				else if (ver.dwMinorVersion == 1)
					return ver.wProductType == VER_NT_WORKSTATION ? "Windows 7" : "Windows Server 2008 R2";
				else if (ver.dwMinorVersion == 2)
					return ver.wProductType == VER_NT_WORKSTATION ? "Windows 8" : "Windows Server 2012";
				else if (ver.dwMinorVersion == 3)
					return ver.wProductType == VER_NT_WORKSTATION ? "Windows 8.1" : "Windows Server 2012 R2";
				break;

			case 10:
				if (ver.dwMinorVersion == 0)
					return ver.wProductType == VER_NT_WORKSTATION ? "Windows 10" : "Windows Server 2016";
				break;

			}

			switch (ver.dwPlatformId) {
			case VER_PLATFORM_WIN32s:
				return "Windows 3.x";
			case VER_PLATFORM_WIN32_WINDOWS:
				return ver.dwMinorVersion == 0 ? "Windows 95" : "Windows 98";
			case VER_PLATFORM_WIN32_NT:
				return "Windows NT";
			}

			return UNKNOWN_OS_NAME;

#elif defined(HVN3_OS_UNIX_BASED)

			struct utsname uts;
			if (uname(&uts) != 0)
				return UNKNOWN_OS_NAME;
			return uts.sysname;

#else

			return UNKNOWN_OS_NAME;

#endif

		}


		PlatformInfo::PlatformInfo(environment::Platform os, const std::string& name, const hvn3::Version& version, const hvn3::Version& service_pack_version, bool is_64_bit) :
			_version(version) {

			_os = os;
			_name = name;
			_is_64_bit = is_64_bit;

		}
		environment::Platform PlatformInfo::Platform() const {

			return _os;

		}
		const std::string& PlatformInfo::Name() const {

			return _name;

		}
		const hvn3::Version& PlatformInfo::ServicePack() const {

			return _service_pack;

		}
		const hvn3::Version& PlatformInfo::Version() const {

			return _version;

		}
		bool PlatformInfo::Is64Bit() const {

			return _is_64_bit;

		}
		bool PlatformInfo::IsUnixBased() const {

			return _os == environment::Platform::Linux || _os == environment::Platform::MacOS || _os == environment::Platform::BSD;

		}

		constexpr environment::Platform OperatingSystem() {

#if defined(HVN3_OS_WINDOWS)
			return Platform::Windows;
#elif defined(HVN3_OS_MACOS)
			return Platform::MacOS;
#elif defined(HVN3_OS_LINUX)
			return Platform::Linux;
#elif defined(HVN3_OS_FREEBSD)
			return Platform::BSD;
#else
			return Platform::Unknown;
#endif

		}
		PlatformInfo OperatingSystemInfo() {

			return PlatformInfo(OperatingSystem(), getOsName(), getOsVersion(), getServicePackVersion(), Is64BitOperatingSystem());

		}
		WindowsVersion GetWindowsVersionFromVersionNumber(const Version& version) {

			switch (version.Major()) {

			case 4:
				if (version.Minor() == 0)
					return WindowsVersion::Windows95;
				if (version.Minor() == 1)
					return WindowsVersion::Windows98;
				if (version.Minor() == 9)
					return WindowsVersion::WindowsME;
				break;

			case 5:
				if (version.Minor() == 0)
					return WindowsVersion::Windows2000;
				if (version.Minor() == 1)
					return WindowsVersion::WindowsXP;
				if (version.Minor() == 2)
					return WindowsVersion::WindowsXpProfessional;
				break;

			case 6:
				if (version.Minor() == 0)
					return WindowsVersion::WindowsVista;
				if (version.Minor() == 1)
					return WindowsVersion::Windows7;
				if (version.Minor() == 2)
					return WindowsVersion::Windows8;
				if (version.Minor() == 3)
					return WindowsVersion::Windows81;
				break;

			case 10:
				if (version.Minor() == 0)
					return WindowsVersion::Windows10;

			}

			return WindowsVersion::Unknown;

		}

	}
}