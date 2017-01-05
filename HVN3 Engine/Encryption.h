#ifndef __ENCRYPTION_H
#define __ENCRYPTION_H
#include <string>
#include "MD5.h"

namespace Encryption {

	std::string MD5(char* filename) {

		MD5Calculator md5;
		return std::string(md5.digestFile(filename));

	}
	
}

#endif