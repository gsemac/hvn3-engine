#pragma once
#include <limits>

namespace hvn3 {

	namespace BitUtils {

		// Returns the size of the given type in bits.
		template <typename T>
		static inline constexpr T Count() {
			return sizeof(T) * CHAR_BIT;
		}
		// Returns the size of the given variable in bits.
		template <typename T>
		static inline constexpr T Count(const T& value) {
			return sizeof(value) * CHAR_BIT;
		}
		// Returns a bit mask at the given index with the given length.
		template <typename T>
		static inline constexpr T Mask(T index, T length) {
			return (~0u >> (Count<T>() - length)) << (Count<T>() - length - index);
		}
		// Copies a range of bits from the source value to the given position in the destination value.
		template <typename T>
		static inline constexpr T Copy(T src, T src_index, T src_length, T dest, T dest_index) {
			// Remember to clear the bits in the destination value before ORing the new ones in.
			return (dest & ~(Mask(0u, src_length) >> dest_index)) | ((src & Mask(src_index, src_length)) << (Count<T>() - src_length - dest_index));
		}
		// Returns n least-significant bits from the input value.
		template <typename T>
		static inline constexpr T LeastSignificantBits(T src, T length) {
			return src & Mask(Count<T>() - length, length);
		}
		template <typename T>
		static inline constexpr T Get(T src, T index, T length) {
			return (src & Mask(index, length)) >> (Count<T>() - index - length);
		}

	};

}