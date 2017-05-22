#ifndef __UDP_CLIENT_H
#define __UDP_CLIENT_H
#include "IDisposable.h"
#include "Packet.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class Connection;

			class UdpClient : public IDisposable {
				friend class UdpListener;

			private:
				unsigned short __port;
				Connection* __connection;

			public:
				UdpClient();
				UdpClient(unsigned short port);
				void Connect(IPAddress address);
				void Connect(IPAddress address, unsigned short port);
				bool Send(Packet& packet);
				int Recieve(Packet& packet);
				void Dispose();

			};

		}
	}
}

#endif