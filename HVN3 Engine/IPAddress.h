#ifndef IP_ADDRESS_H
#define IP_ADDRESS_H
#include <string>
#include "IStringable.h"

namespace Net {

	class IPAddress : public IStringable {

	private:
		unsigned int __address;
		unsigned short __port;

	public:
		IPAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port = 0);
		IPAddress(unsigned int address, unsigned short port = 0);
		IPAddress() {

			__address = 0;
			__port = 0;

		}

		static IPAddress LocalHost(unsigned short port = 0);

		unsigned int Address() const;
		unsigned short Port() const;

		bool operator == (const IPAddress& other) const;
		bool operator != (const IPAddress& other) const;
		
		std::string ToString();

	};

}

#endif