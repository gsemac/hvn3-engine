#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <new>
#include <string>
#include "Packet.h"
#include "NetStream.h"

namespace hvn3 {
	namespace Net {

		// Private

		void Packet::OnWrite() {

			SetWriteMode();

		}
		void Packet::OnRead() {

			SetReadMode();

		}
		void Packet::SetWriteMode() {

			if (__mode == WRITE) return;
			assert(__mode == NONE);

			__ns = new IO::NetStream(IO::NetStream::WRITE, __buf, sizeof(__buf));

			unsigned int type = __type;
			__ns->SerializeInteger(type, 0, ____TYPE_MAX - 1);

			__mode = WRITE;

		}
		void Packet::SetReadMode() {

			if (__mode == READ) return;
			assert(__mode == NONE);

			__ns = new IO::NetStream(IO::NetStream::READ, __buf, sizeof(__buf));

			unsigned int type = UNDEFINED;
			__ns->SerializeInteger(type, 0, ____TYPE_MAX - 1);
			__type = (TYPE)type;

			__mode = READ;

		}

		void Packet::SetRecievedFrom(IPAddress& address) {

			__sender = address;

		}

		// Public

		Packet::Packet() {

			__ns = nullptr;
			__mode = NONE;
			__type = UNDEFINED;
			__sender = IPAddress();
			ClearBuffer();

		}
		Packet::~Packet() {

			if (__ns)
				delete __ns;

		}

		void Packet::SetType(TYPE type) {

			__type = type;

			OnWrite();

		}
		Packet::TYPE Packet::Type() {

			// Allow return type to be returned without read if it has been set.
			if (__type != UNDEFINED)
				return __type;

			OnRead();

			return __type;

		}

		int Packet::Size() {

			// If there is a valid for bytes read (set when recieving packet), return it.
			if (__read_bytes != -1) return __read_bytes;

			// If a read/write stream exists, return the size of the stream.
			if (__ns)
				return __ns->GetDataBytes();
			else
				return 0;

		}
		int Packet::BufferSize() {

			return sizeof(__buf);

		}

		IPAddress& Packet::RecievedFrom() {

			return __sender;

		}

		unsigned char* Packet::GetBuffer() {

			return __buf;

		}
		void Packet::ClearBuffer() {

			// Set contents of buffer to 0.
			memset(__buf, 0, sizeof(__buf));

			// Reset type.
			__type = UNDEFINED;
			__mode = NONE;

			// If there is an active stream, delete it.
			if (__ns != nullptr) {
				delete __ns;
				__ns = nullptr;
			}

			// Clear read bytes (set when reading to a packet).
			__read_bytes = -1;

		}

		Packet& Packet::operator<<(const signed int& value) {
			OnWrite();

			signed int v = value;
			__ns->SerializeInteger(v);

			return *this;
		}
		Packet& Packet::operator<<(const unsigned int& value) {
			OnWrite();

			unsigned int v = value;
			__ns->SerializeInteger(v);

			return *this;

		}
		Packet& Packet::operator<<(const signed short& value) {
			OnWrite();

			signed short v = value;
			__ns->SerializeShort(v);

			return *this;

		}
		Packet& Packet::operator<<(const unsigned short& value) {
			OnWrite();

			unsigned short v = value;
			__ns->SerializeShort(v);

			return *this;

		}
		Packet& Packet::operator<<(const float& value) {
			OnWrite();

			float v = value;
			__ns->SerializeFloat(v);

			return *this;

		}
		Packet& Packet::operator<<(const bool& value) {
			OnWrite();

			bool v = value;
			__ns->SerializeBoolean(v);

			return *this;

		}
		Packet& Packet::operator<<(const char& value) {
			OnWrite();

			char v = value;
			__ns->SerializeByte(v);

			return *this;

		}
		Packet& Packet::operator<<(const signed char& value) {
			OnWrite();

			signed char v = value;
			__ns->SerializeByte(v);

			return *this;

		}
		Packet& Packet::operator<<(const unsigned char& value) {
			OnWrite();

			unsigned char v = value;
			__ns->SerializeByte(v);

			return *this;

		}
		Packet& Packet::operator<<(const char* value) {
			OnWrite();

			// Write the length of the string.
			unsigned int length = strlen(value);
			__ns->SerializeInteger(length);

			// Write the characters of the string.
			const char* p = value;
			while (*p != '\0') {
				char c = *p;
				__ns->SerializeByte(c);
				++p;
			}

			return *this;

		}
		Packet& Packet::operator<<(const std::string value) {
			OnWrite();

			// Write the length of the string.
			unsigned int length = value.length();
			__ns->SerializeInteger(length);

			// Write the characters of the string.
			for (size_t i = 0; i < length; ++i) {
				char c = value[i];
				__ns->SerializeByte(c);
			}

			return *this;

		}

		Packet& Packet::operator >> (signed int& other) {
			OnRead();

			__ns->SerializeInteger(other);

			return *this;

		}
		Packet& Packet::operator >> (unsigned int& other) {
			OnRead();

			__ns->SerializeInteger(other);

			return *this;

		}
		Packet& Packet::operator >> (signed short& other) {
			OnRead();

			__ns->SerializeShort(other);

			return *this;

		}
		Packet& Packet::operator >> (unsigned short& other) {
			OnRead();

			__ns->SerializeShort(other);

			return *this;

		}
		Packet& Packet::operator >> (float& other) {
			OnRead();

			__ns->SerializeFloat(other);

			return *this;

		}
		Packet& Packet::operator >> (bool& other) {
			OnRead();

			__ns->SerializeBoolean(other);

			return *this;

		}
		Packet& Packet::operator >> (char& other) {
			OnRead();

			__ns->SerializeByte(other);

			return *this;

		}
		Packet& Packet::operator >> (signed char& other) {
			OnRead();

			__ns->SerializeByte(other);

			return *this;

		}
		Packet& Packet::operator >> (unsigned char& other) {
			OnRead();

			__ns->SerializeByte(other);

			return *this;

		}
		Packet& Packet::operator >> (std::string& other) {
			OnRead();

			// Read the length of the string.
			unsigned int length;
			__ns->SerializeInteger(length);

			// Clear the output string.
			other.clear();

			// Read the characters of the string.
			for (size_t i = 0; i < length; ++i) {
				char c;
				__ns->SerializeByte(c);
				other.push_back(c);
			}

			return *this;

		}

		bool SendPacket(Sockets::Socket& socket, IPAddress& endpoint, Packet& packet) {

			if (endpoint.Address() == 0)
				return false;

			int size = packet.Size();

			unsigned char* data = new unsigned char[size + 4];
			data[0] = (unsigned char)(PROTOCOL_ID >> 24);
			data[1] = (unsigned char)((PROTOCOL_ID >> 16) & 0xFF);
			data[2] = (unsigned char)((PROTOCOL_ID >> 8) & 0xFF);
			data[3] = (unsigned char)((PROTOCOL_ID)& 0xFF);
			memcpy(&data[4], packet.GetBuffer(), size);
			bool res = socket.Send(endpoint, data, size + 4);
			delete[] data;

			return res;

		}
		int RecievePacket(Sockets::Socket& socket, Packet& packet) {

			// Read data from the socket.
			IPAddress sender;
			unsigned char* data = new unsigned char[PACKET_BUFFER_SIZE + 4];
			int bytes_read = socket.Receive(sender, data, PACKET_BUFFER_SIZE + 4);

			// If no bytes were read, return.
			if (bytes_read == 0) {
				delete[] data;
				return 0;
			}

			// If the packet was too small to contain our protocol id, return.
			if (bytes_read <= 4) {
				delete[] data;
				return 0;
			}

			// Check that the packet contains our protocol id.
			if (data[0] != (unsigned char)(PROTOCOL_ID >> 24) ||
				data[1] != (unsigned char)((PROTOCOL_ID >> 16) & 0xFF) ||
				data[2] != (unsigned char)((PROTOCOL_ID >> 8) & 0xFF) ||
				data[3] != (unsigned char)(PROTOCOL_ID & 0xFF)) {
				delete[] data;
				return 0;
			}

			// Clear existing data from the packet.
			packet.ClearBuffer();

			// Copy the data into the packet.
			memcpy(packet.GetBuffer(), &data[4], packet.BufferSize() - 4);
			delete[] data;

			// Add additional parameters to packet.
			packet.__read_bytes = bytes_read - 4;
			packet.__sender = sender;

			// Return the size (minus the protocol id).
			return bytes_read - 4;

		}

	}
}