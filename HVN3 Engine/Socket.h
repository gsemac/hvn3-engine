#ifndef __SOCKET_H
#define __SOCKET_H
#include "IPAddress.h"

// The following cross-platform socket code was adapted from
// https://github.com/ThisIsRobokitty/netgame/blob/master/01%20-%20Sending%20and%20Receiving%20Packets/Net.h

namespace Net {
	namespace Sockets {

		// The following functions require an equal number of calls; a counter is used to ensure balance before freeing resources.

		// Initializes sockets for use.
		inline bool InitializeSockets();

		// Frees resources used by sockets.
		inline void ShutdownSockets();

		class Socket {

		private:
			int __handle;

		public:
			Socket();
			~Socket();

			bool Open(unsigned short port);
			void Close();

			bool IsOpen() const;

			bool Send(const IPAddress& destination, const void* data, int size);
			int Receive(IPAddress& sender, void* data, int size);

		};

	}

	void sleep(int milliseconds);

}

#endif