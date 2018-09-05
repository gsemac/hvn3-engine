#pragma once

// Check for endianness macros across various compilers.
// A list of macros is available here: http://sourceforge.net/p/predef/wiki/Endianness/
// Note that not all compilers will provide them (MSVC does not, for example).
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) \
|| defined(_M_PPC) // For MSVC, check architectures known to be big endian.
#define HVN3_BIG_ENDIAN
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) \
|| defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || defined(_M_ARM) // For MSVC, check architectures known to be little endian.
#define HVN3_LITTLE_ENDIAN
#endif

namespace hvn3 {
	namespace environment {

		enum class Endian {
			Unknown,
			Big,
			Little,
			BigWord,
			LittleWord
		};

		// Returns the endianness of the current machine.
		Endian Endianness();

	}
}