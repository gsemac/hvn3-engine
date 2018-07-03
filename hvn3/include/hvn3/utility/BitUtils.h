#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

namespace hvn3 {

	typedef uint8_t Byte;

	namespace BitUtils {

		// Returns the size of the given type in bits.
		template <typename T>
		inline constexpr T Count() {
			return sizeof(T) * CHAR_BIT;
		}
		// Returns the size of the given variable in bits.
		template <typename T>
		constexpr T Count(const T& value) {
			return sizeof(value) * CHAR_BIT;
		}
		// Returns a bit mask at the given index with the given length.
		template <typename T>
		inline constexpr T Mask(T index, T length) {
			return (~0u >> (Count<T>() - length)) << (Count<T>() - length - index);
		}
		// Copies a range of bits from the source value to the given position in the destination value.
		template <typename T>
		inline constexpr T Copy(T src, T src_index, T src_length, T dest, T dest_index) {
			// Remember to clear the bits in the destination value before ORing the new ones in.
			return (dest & ~(Mask(0u, src_length) >> dest_index)) | ((src & Mask(src_index, src_length)) << (Count<T>() - src_length - dest_index));
		}
		// Returns n least-significant bits from the input value.
		template <typename T>
		inline constexpr T LeastSignificantBits(T src, T length) {
			return src & Mask(Count<T>() - length, length);
		}
		template <typename T>
		inline constexpr T Get(T src, T index, T length) {
			return (src & Mask(index, length)) >> (Count<T>() - index - length);
		}
		template <typename T>
		inline constexpr bool Get(T from, T index) {
			return static_cast<bool>((from >> (Count(from) - 1u - index)) & 1u);
		}

		// Returns the number of bytes required to store the given number of bits.
		unsigned int CountBytes(unsigned int bits);

		// Returns the number of bits required to represent the given number of distinct values.
		int BitsRequired(uint32_t distinct_values);
		// Returns the number of bits required to represent a value between the given min and max values (inclusive).
		int BitsRequired(uint32_t min, uint32_t max);

	};

}