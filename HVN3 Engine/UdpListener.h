#ifndef __UDP_LISTENER_H
#define __UDP_LISTENER_H
#include <queue>
#include "UdpClient.h"

namespace Net {
	namespace Sockets {

		class UdpListener : public IDisposable {

		private:
			unsigned short __port;
			std::deque<IPAddress> __pending_connections;
			UdpClient __client;

		protected:
			virtual void ProcessPacket(Packet& packet);
			std::deque<IPAddress>& PendingConnections() {

				return __pending_connections;

			}

		public:
			UdpListener(unsigned short port);

			void Listen();

			bool Pending();
			
			void Dispose();

		};

	}
}


#endif