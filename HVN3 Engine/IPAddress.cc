#include <sstream>
#include "IPAddress.h"

namespace Net {

	IPAddress::IPAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port) {

		__address = (a << 24) | (b << 16) | (c << 8) | d;
		__port = port;

	}
	IPAddress::IPAddress(unsigned int address, unsigned short port) {

		__address = address;
		__port = port;

	}

	IPAddress IPAddress::LocalHost(unsigned short port) {

		return IPAddress(127, 0, 0, 1, port);

	}

	unsigned int IPAddress::Address() const {

		return __address;

	}
	unsigned short IPAddress::Port() const {

		return __port;

	}

	bool IPAddress::operator == (const IPAddress& other) const {

		return (__address == other.__address) && (__port == other.__port);

	}
	bool IPAddress::operator != (const IPAddress& other) const {

		return  !(*this == other);

	}

	std::string IPAddress::ToString() {

		std::stringstream stream;

		stream << (unsigned int)(unsigned char)(__address >> 24) << ".";
		stream << (unsigned int)(unsigned char)(__address >> 16) << ".";
		stream << (unsigned int)(unsigned char)(__address >> 8) << ".";
		stream << (unsigned int)(unsigned char)(__address);

		if (__port)
			stream << ":" << __port;

		return stream.str();

	}

}
