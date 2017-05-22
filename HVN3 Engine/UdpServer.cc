#include <assert.h>
#include <stdio.h>
#include "UdpServer.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			// Protected

			void UdpServer::HandleConnectionRequest(Packet& packet) {
				if (!AutoAccept) return;

				if (__clients.size() < __max_connections) {
					// Accept the connection into our list of clients.
					AcceptClient(packet.RecievedFrom());
				}
				else {
					// No more connection slots left; send rejection packet.
					Packet reject;
					reject.SetType(Packet::CONNECTION_FAILED);
					reject << "Unable to connect to server (no free slots available).";
					SendPacket(__server_socket, packet.RecievedFrom(), reject);
				}

			}

			void UdpServer::AcceptClient(IPAddress& endpoint) {

				ClientInfo info;
				info.EndPoint = endpoint;
				time(&info.LastHeardFrom);

				__clients[info.EndPoint.ToString()] = info;

				printf("Accepted new connection from %s\n", endpoint.ToString().c_str());

			}
			void UdpServer::DropClient(IPAddress& endpoint) {

				__clients.erase(endpoint.ToString());

			}

			// Public

			UdpServer::UdpServer(unsigned short port) {

				AutoAccept = true;

				__server_address = IPAddress(127, 0, 0, 1, port);
				__max_connections -= 1;
				__running = false;
				__timeout = 5.0f;

			}
			UdpServer::~UdpServer() {

			}

			bool UdpServer::Start() {

				// Make sure that the socket isn't already open.
				if (__running) return true;

				// Open the socket to begin listening for new packets.
				__running = __server_socket.Open(__server_address.Port());

				if (__running)
					printf("Server started on port %d\n", __server_address.Port());

				return __running;

			}

			bool UdpServer::Broadcast(Packet& packet) {
				if (!__running) Start();

				// Initialize current time variable to check for timeouts.
				time_t now;
				time(&now);

				// Broadcast the packet to all connected clients.
				for (auto it = __clients.begin(); it != __clients.end();) {

					ClientInfo& info = it->second;
					++it;

					// Drop the client if the connection has timed out.
					if (difftime(now, info.LastHeardFrom) > __timeout) {
						printf("Dropped client %s (timed out)\n", info.EndPoint.ToString().c_str());
						DropClient(info.EndPoint);
						continue;
					}

					SendPacket(__server_socket, info.EndPoint, packet);

				}

				return true;

			}
			int UdpServer::Recieve(Packet& packet) {
				if (!__running) Start();

				// Check for any incoming packets.
				int bytes_read = RecievePacket(__server_socket, packet);

				// If a packet was read, handle it accordingly.
				if (bytes_read > 0) {

					// Update the "last heard from" value for the client.
					time(&(__clients[packet.RecievedFrom().ToString()].LastHeardFrom));

					// Attempt to handle the packet.
					switch (packet.Type()) {

					case Packet::CONNECTION_REQUEST:
						HandleConnectionRequest(packet);
						break;

					}
				}

				// Unhandled packets will be handled by the caller.
				return bytes_read;

			}

			void UdpServer::SetMaxConnections(int max) {

				__max_connections = max;

			}
			void UdpServer::SetTimeout(float timeout) {

				__timeout = timeout;

			}

		}
	}
}