#include "NetworkConfig.h"

// Unique protocol ID for your project. Ensures that only packets intended for the project are accepted.
const unsigned int Net::PROTOCOL_ID = 0x99887766;

// Default packet starting buffer size (resizes as needed).
const int Net::PACKET_BUFFER_SIZE = 256;