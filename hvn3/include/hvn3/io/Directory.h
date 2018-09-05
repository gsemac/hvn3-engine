#pragma once
#include <string>

namespace hvn3 {

	namespace IO {

		class Directory {

		public:
			static const std::string GetCurrentDirectory();
			static const std::string GetTemporaryDirectory();
			static void SetCurrentDirectory(const std::string path);

		};

	}

}