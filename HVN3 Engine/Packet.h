#ifndef __PACKET_H
#define __PACKET_H
#include "Socket.h"
#include "NetworkConfig.h"

namespace Net {

	namespace IO {
		class NetStream;
	}
	namespace Sockets {
		class UdpClient;
	}
			
	class Packet {
		friend class Sockets::UdpClient;
		friend int RecievePacket(Sockets::Socket& socket, Packet& packet);

	public:
		enum TYPE {
			UNDEFINED,
			CONNECTION_REQUEST,
			CONNECTION_FAILED,
			____TYPE_MAX
		};

	private:
		enum MODE {
			READ,
			WRITE,
			NONE
		};

		unsigned char __buf[256];
		IO::NetStream* __ns;
		MODE __mode;
		TYPE __type;
		IPAddress __sender;

		int __read_bytes;

		void OnWrite();
		void OnRead();
		void SetWriteMode();
		void SetReadMode();

		void SetRecievedFrom(IPAddress& address);

	public:
		Packet();
		~Packet();

		void SetType(TYPE type);
		
		TYPE Type();
		int Size();
		int BufferSize();
		IPAddress& RecievedFrom();

		unsigned char* GetBuffer();
		void ClearBuffer();

		Packet& operator<<(const signed int& value);
		Packet& operator<<(const unsigned int& value);
		Packet& operator<<(const signed short& value);
		Packet& operator<<(const unsigned short& value);
		Packet& operator<<(const float& value);
		Packet& operator<<(const bool& value);
		Packet& operator<<(const char& value);
		Packet& operator<<(const signed char& value);
		Packet& operator<<(const unsigned char& value);
		Packet& operator<<(const char* value);
		Packet& operator<<(const std::string value);

		Packet& operator >> (signed int& other);
		Packet& operator >> (unsigned int& other);
		Packet& operator >> (signed short& other);
		Packet& operator >> (unsigned short& other);
		Packet& operator >> (float& other);
		Packet& operator >> (bool& other);
		Packet& operator >> (char& other);
		Packet& operator >> (signed char& other);
		Packet& operator >> (unsigned char& other);
		Packet& operator >> (std::string& other);

	};

	bool SendPacket(Sockets::Socket& socket, IPAddress& endpoint, Packet& packet);
	int RecievePacket(Sockets::Socket& socket, Packet& packet);

}

#endif