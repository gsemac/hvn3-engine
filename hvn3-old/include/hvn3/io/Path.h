#pragma once
#include <string>

namespace hvn3 {
	namespace IO {

		class Path {

		public:
			static std::string Combine(const std::string& path1, const std::string& path2);
			static std::string Combine(const std::string& path1, const std::string& path2, const std::string& path3);
			static std::string Combine(const std::string& path1, const std::string& path2, const std::string& path3, const std::string& path4);
			static bool IsPathRooted(const std::string& path);
			static char DirectorySeparatorChar();
			static char AltDirectorySeparatorChar();
			static char VolumeSeparatorChar();
			static std::string GetExtension(const std::string& path);
			static std::string SetExtension(const std::string& path, const std::string& ext);
			static std::string GetFileName(const std::string& path);
			static std::string GetDirectoryName(const std::string& path);
			static std::string GetTemporaryFilePath();

		private:
			static std::string::size_type _getExtensionPos(const std::string& path);

		};

	}
}