#include "Socket.h"

// Platform detection
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib" )
#pragma warning( disable : 4996  ) // get rid of all secure crt warning. (sscanf_s)

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#else

#error Unknown platform!

#endif

#include <assert.h>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			inline bool InitializeSockets() {

#if PLATFORM == PLATFORM_WINDOWS
				WSADATA WsaData;
				return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
				return true;
#endif

			}
			inline void ShutdownSockets() {

#if PLATFORM == PLATFORM_WINDOWS
				WSACleanup();
#endif

			}

			Socket::Socket() {

				// Initialize members.
				__handle = 0;

			}
			Socket::~Socket() {

				// Close the socket.
				Close();

			}

			bool Socket::Open(unsigned short port) {
				assert(!IsOpen());

				// Initialize sockets.
				InitializeSockets();

				// Get a socket handle from the OS.
				__handle = (int)::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

				if (socket <= 0) {
					printf("failed to create socket\n");
					__handle = 0;
					return false;
				}

				// Bind the socket to a port.
				sockaddr_in address;
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = INADDR_ANY;
				address.sin_port = htons((unsigned short)port);

				if (bind(__handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
					printf("failed to bind socket\n");
					Close();
					return false;
				}

				// Set non-blocking IO.
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

				int nonBlocking = 1;
				if (fcntl(__handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
					printf("failed to set non-blocking socket\n");
					Close();
					return false;
				}

#elif PLATFORM == PLATFORM_WINDOWS

				DWORD nonBlocking = 1;
				if (ioctlsocket(__handle, FIONBIO, &nonBlocking) != 0) {
					printf("failed to set non-blocking socket\n");
					Close();
					return false;
				}

#endif

				// The socket was opened successfully.
				return true;

			}
			void Socket::Close() {

				// Shutdown sockets.
				ShutdownSockets();

				// Close the socket (if a handle has been created).
				if (__handle != 0) {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
					close(__handle);
#elif PLATFORM == PLATFORM_WINDOWS
					closesocket(__handle);
#endif
					__handle = 0;
				}

			}

			bool Socket::IsOpen() const {

				return __handle != 0;

			}
			bool Socket::Send(const IPAddress& destination, const void * data, int size) {
				assert(data);
				assert(size > 0);

				if (__handle == 0)
					return false;

				sockaddr_in address;
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = htonl(destination.Address());
				address.sin_port = htons((unsigned short)destination.Port());

				int sent_bytes = sendto(__handle, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

				return sent_bytes == size;

			}
			int Socket::Receive(IPAddress& sender, void * data, int size) {
				assert(data);
				assert(size > 0);

				if (__handle == 0)
					return false;

#if PLATFORM == PLATFORM_WINDOWS
				typedef int socklen_t;
#endif

				sockaddr_in from;
				socklen_t fromLength = sizeof(from);

				int received_bytes = recvfrom(__handle, (char*)data, size, 0, (sockaddr*)&from, &fromLength);

				if (received_bytes <= 0)
					return 0;

				unsigned int address = ntohl(from.sin_addr.s_addr);
				unsigned int port = ntohs(from.sin_port);

				sender = IPAddress(address, port);

				return received_bytes;
			}

		}

#if PLATFORM == PLATFORM_WINDOWS

		void sleep(int milliseconds) {
			Sleep(milliseconds);
		}

#else

#include <unistd.h>
		void sleep(int milliseconds) { usleep((int)(milliseconds * 1000.0f)); }

#endif

	}
}