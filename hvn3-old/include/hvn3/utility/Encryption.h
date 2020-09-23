#pragma once
#include <string>
#include "hvn3/MD5.h"

/*
Yes, I know, hashing != encrypting.
In the future, this namespace will contain more comprehensive hashing and encryption algorithms.
*/

namespace hvn3 {
	namespace Encryption {

		std::string MD5(char* filename) {

			MD5Calculator md5;
			return std::string(md5.digestFile(filename));

		}

	}
}