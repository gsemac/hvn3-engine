#include "hvn3/exceptions/Exception.h"

namespace hvn3 {

	namespace IO {

		class IOException :
			public Exception {

		public:
			using Exception::Exception;

			IOException();

		};

		class FileNotFoundException :
			public IOException {

		public:
			FileNotFoundException(const std::string& filename);

		};

	}

}