#include "UdpListener.h"
#include "Connection.h"

namespace Net {
	namespace Sockets {

		// Protected

		void UdpListener::ProcessPacket(Packet& packet) {

			// Set the client back to listening mode (to continue listening for new packets).
			__client.__connection->Listen();

			// Ignore all packets that aren't a connection request.
			if (packet.Type() != Packet::CONNECTION_REQUEST)
				return;

			// Add the sender to the list of pending connections.
			__pending_connections.push_back(packet.RecievedFrom());

		}

		// Public

		UdpListener::UdpListener(unsigned short port) {

			__port = port;

		}

		void UdpListener::Listen() {

			// Initialize a connection through the UdpClient object.
			if (!__client.__connection)
				__client.__connection = new Connection(__port, 5.0f);

			// Start listening on the new connection.
			if (!__client.__connection->IsListening()) {
				__client.__connection->Start(__port);
				__client.__connection->Listen();
			}
			
		}
		bool UdpListener::Pending() {

			// If a connection isn't running, return false.
			if (!__client.__connection || !__client.__connection->IsListening())
				return false;

			// Process all connection requests.
			Packet p;
			while (__client.Recieve(p))
				ProcessPacket(p);

			// Return whether any connection requests are pending.
			return (!__pending_connections.empty());
			
		}

		void UdpListener::Dispose() {

			__client.Dispose();

		}

	}
}