#pragma once

// List of operating system definitions:
// https://sourceforge.net/p/predef/wiki/OperatingSystems/

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define HVN3_OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#define HVN3_OS_MACOS
#elif defined(__linux) || defined(__linux__) || defined(linux) || defined(__gnu_linux__)
#define HVN3_OS_LINUX
#elif defined(__unix) || defined(__unix__)
#define HVN3_OS_UNIX
#elif defined(__FreeBSD__)
#define HVN3_OS_FREEBSD
#endif

// Check 32-bit/64-bit for Windows
#if defined(HVN3_OS_WINDOWS)
#if _WIN64
#define HVN3_OS_X64
#else
#define HVN3_OS_X86
#endif
#endif

// Check 32-bit/64-bit for GCC
#if defined(__GNUC__)
#if defined(__x86_64__) || defined(__ppc64__)
#define HVN3_OS_X64
#else
#define HVN3_OS_X86
#endif
#endif

#if defined(HVN3_OS_LINUX) || defined(HVN3_OS_UNIX) || defined(HVN3_OS_MACOSX) || defined(HVN3_OS_FREEBSD)
#define HVN3_OS_UNIX_BASED
#endif