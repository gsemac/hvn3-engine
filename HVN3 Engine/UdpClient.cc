#include <new>
#include <string.h>
#include "UdpClient.h"
#include "Connection.h"
#include "NetworkConfig.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			// Private


			// Public

			UdpClient::UdpClient() : UdpClient(0) {}
			UdpClient::UdpClient(unsigned short port) {

				__port = port;
				__connection = nullptr;

			}

			void UdpClient::Connect(IPAddress address) {

				Connect(address, address.Port());

			}
			void UdpClient::Connect(IPAddress address, unsigned short port) {

				// Create a new connection if one doesn't already exist.
				if (__connection == nullptr) {
					__connection = new Connection(PROTOCOL_ID, 5.0f);
					__connection->Start(__port);
				}

				// Begin a connection attempt to the remote endpoint.
				IPAddress endpoint(address.Address(), port);
				__connection->Connect(endpoint);

				// Send a connection request.
				Packet req;
				req.SetType(Packet::CONNECTION_REQUEST);
				Send(req);

			}

			bool UdpClient::Send(Packet& packet) {

				if (!__connection)
					return false;

				// Contruct a temporary buffer to hold the packet data, and copy the data into it.
				int size = packet.Size();
				unsigned char* data = new unsigned char[size];
				memcpy(data, packet.GetBuffer(), size);

				// Send the packet data.
				bool result = __connection->SendPacket(data, size);

				// Delete the temporary buffer.
				delete[] data;

				// Return the result.
				return result;

			}
			int UdpClient::Recieve(Packet& packet) {

				if (!__connection)
					return 0;

				// Attempt to recieve packet data.
				unsigned char buf[256];
				int size = __connection->ReceivePacket(buf, sizeof(buf));
				if (size == 0) return 0;

				// Clear the packet's buffer in case there's anything in it.
				packet.ClearBuffer();

				// Copy the data buffer into the packet's buffer.
				memcpy(packet.GetBuffer(), buf, size);
				packet.SetRecievedFrom(__connection->LastRecievedFrom());

				packet.__ns = nullptr;

				// Return the number of bytes read.
				return size;

			}

			void UdpClient::Dispose() {

				if (__connection) {
					__connection->Stop();
					delete __connection;
					__connection = nullptr;
				}

			}

		}
	}
}