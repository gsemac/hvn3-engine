#include "hvn3/utility/BitUtils.h"

#include <cmath>

namespace hvn3 {
	namespace BitUtils {

		// Returns the number of bytes required to store the given number of bits.
		unsigned int CountBytes(unsigned int bits) {
			return static_cast<unsigned int>((std::ceil)(bits / static_cast<float>(CHAR_BIT)));
		}

		// Returns the number of bits required to represent the given number of distinct values.
		int BitsRequired(uint32_t distinct_values) {

			// (It doesn't make sense to have 0 distinct values.)
			assert(distinct_values > 1);

			// Calculate the maximum value that can be represented-- This means all bits are equal to 1.
			unsigned int max = distinct_values - 1;

			// Loop until we reach a 0 bit, or expend all 32 bits.
			for (int index = 0; index < 32; ++index) {

				if ((max & ~1) == 0)
					return index + 1;
				max >>= 1;

			}

			// If we haven't returned yet, we require the full 32 bits.
			return 32;

		}
		// Returns the number of bits required to represent a value between the given min and max values (inclusive).
		int BitsRequired(uint32_t min, uint32_t max) {

			// Make sure that max and min values are valid.
			assert(max > min);
			assert(max >= 1);

			// 0x7FFFFFF is the maximum value of a (signed) 32-bit integer.
			// If the difference between the max/min is this high, we require a full 32 bits. 
			if (max - min >= 0x7FFFFFF)
				return 32;

			// Otherwise, calculate the bits required to represent n distinct values,
			// where n is the range bounded by min and max, plus 0.
			return BitsRequired(max - min + 1);

		}

	}
}