#ifndef CONNECTION_H
#define CONNECTION_H
#include "Socket.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class Connection {

			public:
				enum MODE {
					NONE,
					CLIENT,
					SERVER
				};

			private:
				enum STATE {
					DISCONNECTED,
					LISTENING,
					CONNECTING,
					CONNECTION_FAILURE,
					CONNECTED
				};

				unsigned int __protocol_id;

				float __timeout;
				float __timeout_accumulator;
				bool __running;
				MODE __mode;
				STATE __state;
				Socket __socket;
				IPAddress __remote_address;
				IPAddress __last_sender;

				// Disconnects and clears all associated information.
				void ClearData();

			protected:
				virtual void OnStart();
				virtual void OnStop();
				virtual void OnConnect();
				virtual void OnDisconnect();

			public:
				Connection(unsigned int protocolId, float timeout);
				virtual ~Connection();

				bool Start(int port);
				void Stop();
				virtual void Update(float delta_time);

				void Listen();
				void Connect(const IPAddress& address);

				bool IsRunning() const;
				bool IsConnecting() const;
				bool ConnectFailed() const;
				bool IsConnected() const;
				bool IsListening() const;

				MODE GetMode() const;
				int GetHeaderSize() const;

				IPAddress& LastRecievedFrom();

				virtual bool SendPacket(const unsigned char data[], int size);
				virtual int ReceivePacket(unsigned char data[], int size);

			};

		}
	}
}

#endif