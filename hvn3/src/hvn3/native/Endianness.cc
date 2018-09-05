#include "hvn3/native/Endianness.h"
#include <cstdint>

namespace hvn3 {
	namespace environment {

		Endian Endianness() {

			union {
				uint32_t value;
				uint8_t data[sizeof(uint32_t)];
			} number;

			number.data[0] = 0x00;
			number.data[1] = 0x01;
			number.data[2] = 0x02;
			number.data[3] = 0x03;

			switch (number.value) {

			case UINT32_C(0x00010203): 
				return Endian::Big;
			case UINT32_C(0x03020100):
				return Endian::Little;
			case UINT32_C(0x02030001): 
				return Endian::BigWord;
			case UINT32_C(0x01000302): 
				return Endian::LittleWord;
			default:                   
				return Endian::Unknown;

			}

		}

	}
}