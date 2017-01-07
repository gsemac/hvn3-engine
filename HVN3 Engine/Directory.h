#pragma once
#include <string>

namespace IO {

	class Directory {

	public:
		static const std::string GetCurrentDirectory();
		static void SetCurrentDirectory(const std::string path);

	private:
		static std::string __current_directory;

	};

}