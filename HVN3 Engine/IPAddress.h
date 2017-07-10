#pragma once
#include <string>
#include <iostream>

namespace hvn3 {
	namespace Net {

		class IPAddress {

		public:
			IPAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
			IPAddress(unsigned int address);
			IPAddress(const char* address);

			// Returns object representing the address 127.0.0.1.
			static IPAddress LocalHost();

			// Returns integer representation of the address.
			unsigned int Address() const;

			bool operator==(const IPAddress& other) const;
			bool operator!=(const IPAddress& other) const;

			// Returns string representation of the address.
			std::string ToString() const;

		private:
			unsigned int _address;

		};

		std::ostream& operator << (std::ostream& stream, const IPAddress& value);

	}
}