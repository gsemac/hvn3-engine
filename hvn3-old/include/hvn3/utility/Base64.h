#pragma once
#include <string>
#include <vector>

namespace hvn3 {

	typedef unsigned char Byte;

	class Base64 {

	public:
		// Converts a base64 string to an array of bytes.
		static std::vector<Byte> Decode(const std::string& in);
		// Converts a base64-encoded string to an ASCII-encoded string.
		static std::string DecodeString(const std::string& in);

	};

}