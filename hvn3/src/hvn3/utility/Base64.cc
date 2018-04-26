#include "hvn3/utility/Base64.h"
#include <iostream>

namespace hvn3 {

	// Helper functions

	Byte base64_decode_char(char c) {

		if (c >= '0' && c <= '9')
			return c + 4;

		if (c >= 'A' && c <= 'Z')
			return c - 65;

		if (c >= 'a' && c <= 'z')
			return c - 71;

		if (c == '+')
			return 62;

		if (c == '/')
			return 63;

	}



	std::vector<Byte> Base64::Decode(const std::string& in) {

		// Reserve enough space for the decoded bytes.
		std::vector<Byte> out;
		out.reserve(in.size() * 3 / 4);

		Byte next_byte = 0;
		unsigned char bit_offset = 0;

		for (std::string::size_type i = 0; i < in.size(); ++i) {

			if (in[i] == '=')
				break;

			Byte c = base64_decode_char(in[i]);

			// Copy as many of the 6 bits to the first byte as possible.
			next_byte |= (c << 2) >> bit_offset;
			bit_offset += 6;

			// If not all 6 bits could fit in this byte, copy the rest to the next byte.
			if (bit_offset >= CHAR_BIT) {

				out.push_back(next_byte);

				next_byte = 0;
				bit_offset -= CHAR_BIT;

				if (bit_offset != 0)
					next_byte |= c << (2 + (6 - bit_offset));

			}

		}

		// Add the last byte if it has been written to.
		if (next_byte != 0)
			out.push_back(next_byte);

		// Return the result.
		return out;

	}
	std::string Base64::DecodeString(const std::string& in) {

		std::vector<Byte> bytes = Decode(in);
		
		std::string out;
		out.reserve(in.size() * 4 / 3);

		for (auto i = bytes.begin(); i != bytes.end(); ++i)
			out.push_back(*i);

		return out;

	}

}