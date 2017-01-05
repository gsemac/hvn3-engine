#include <unordered_map>
#include <time.h>
#include "Packet.h"

namespace Net {
	namespace Sockets {

		class UdpServer {

		private:
			struct ClientInfo {
				IPAddress EndPoint;
				time_t LastHeardFrom;
			};

			std::unordered_map<std::string, ClientInfo> __clients;

			IPAddress __server_address;
			Socket __server_socket;

			int __max_connections;
			float __timeout;

			bool __running;

		protected:
			virtual void HandleConnectionRequest(Packet& packet);

			void AcceptClient(IPAddress& endpoint);
			void DropClient(IPAddress& endpoint);

		public:
			// Whether or not to automatically handle new connection requests.
			bool AutoAccept;

			UdpServer(unsigned short port);
			~UdpServer();

			bool Start();

			bool Broadcast(Packet& packet);
			virtual int Recieve(Packet& packet);

			// Sets the maximum number of allowed connections to this server instance.
			void SetMaxConnections(int max);

			void SetTimeout(float timeout);

		};

	}
}