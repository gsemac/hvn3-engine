#include <assert.h>
#include <string.h>
#include "Connection.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			// Private

			void Connection::ClearData() {

				__state = DISCONNECTED;
				__timeout_accumulator = 0.0f;
				__remote_address = IPAddress();

			}

			// Protected

			void Connection::OnStart() {}
			void Connection::OnStop() {}
			void Connection::OnConnect() {}
			void Connection::OnDisconnect() {}

			// Public

			Connection::Connection(unsigned int protocolId, float timeout) {

				__protocol_id = protocolId;
				__timeout = timeout;
				__mode = NONE;
				__running = false;
				__last_sender = IPAddress();

				ClearData();

			}
			Connection::~Connection() {

				if (IsRunning())
					Stop();

			}

			bool Connection::Start(int port) {

				assert(!__running);
				printf("start connection on port %d\n", port);

				if (!__socket.Open(port))
					return false;
				__running = true;
				OnStart();

				return true;

			}
			void Connection::Stop() {

				assert(__running);
				printf("stop connection\n");

				bool connected = IsConnected();
				ClearData();
				__socket.Close();
				__running = false;
				if (connected)
					OnDisconnect();

				OnStop();

			}
			void Connection::Update(float deltaTime) {

				assert(__running);

				__timeout_accumulator += deltaTime;

				if (__timeout_accumulator > __timeout) {

					if (__state == CONNECTING) {

						printf("connect timed out\n");
						ClearData();
						__state = CONNECTION_FAILURE;
						OnDisconnect();

					}
					else if (__state == CONNECTED) {

						printf("connection timed out\n");
						ClearData();
						if (__state == CONNECTING)
							__state = CONNECTION_FAILURE;
						OnDisconnect();

					}

				}

			}

			void Connection::Listen() {

				printf("server listening for connection\n");

				bool connected = IsConnected();
				ClearData();
				if (connected)
					OnDisconnect();
				__mode = SERVER;
				__state = LISTENING;

			}
			void Connection::Connect(const IPAddress& address) {

				bool connected = IsConnected();
				ClearData();
				if (connected)
					OnDisconnect();
				__mode = CLIENT;
				__state = CONNECTING;
				__remote_address = address;

			}

			bool Connection::IsRunning() const {

				return __running;

			}
			bool Connection::IsConnecting() const {

				return __state == CONNECTING;

			}
			bool Connection::ConnectFailed() const {

				return __state == CONNECTION_FAILURE;

			}
			bool Connection::IsConnected() const {

				return __state == CONNECTED;

			}
			bool Connection::IsListening() const {

				return __state == LISTENING;

			}

			Connection::MODE Connection::GetMode() const {

				return __mode;

			}
			int Connection::GetHeaderSize() const {

				return 4;

			}

			IPAddress& Connection::LastRecievedFrom() {

				return __last_sender;

			}

			bool Connection::SendPacket(const unsigned char data[], int size) {

				assert(__running);

				if (__remote_address.Address() == 0)
					return false;

				unsigned char* packet = new unsigned char[size + 4];
				packet[0] = (unsigned char)(__protocol_id >> 24);
				packet[1] = (unsigned char)((__protocol_id >> 16) & 0xFF);
				packet[2] = (unsigned char)((__protocol_id >> 8) & 0xFF);
				packet[3] = (unsigned char)((__protocol_id)& 0xFF);
				memcpy(&packet[4], data, size);
				bool res = __socket.Send(__remote_address, packet, size + 4);
				delete[] packet;

				return res;

			}
			int Connection::ReceivePacket(unsigned char data[], int size) {

				assert(__running);

				unsigned char* packet = new unsigned char[size + 4];

				int bytes_read = __socket.Receive(__last_sender, packet, size + 4);
				if (bytes_read == 0) {
					delete[] packet;
					return 0;
				}
				if (bytes_read <= 4) {
					delete[] packet;
					return 0;
				}
				if (packet[0] != (unsigned char)(__protocol_id >> 24) ||
					packet[1] != (unsigned char)((__protocol_id >> 16) & 0xFF) ||
					packet[2] != (unsigned char)((__protocol_id >> 8) & 0xFF) ||
					packet[3] != (unsigned char)(__protocol_id & 0xFF)) {
					delete[] packet;
					return 0;
				}
				if (__mode == SERVER && !IsConnected()) {
					__state = CONNECTED;
					__remote_address = __last_sender;
					OnConnect();
				}
				if (__last_sender == __remote_address) {
					if (__mode == CLIENT && __state == CONNECTING) {
						printf("client completes connection with server\n");
						__state = CONNECTED;
						OnConnect();
					}
					__timeout_accumulator = 0.0f;
					memcpy(data, &packet[4], size - 4);
					delete[] packet;
					return size - 4;
				}

				delete[] packet;

				return 0;

			}

		}
	}
}