#pragma once
#include <allegro5/allegro.h>
#include <vector>
#include <string>

namespace IO {

	enum class TextEncoding {
		UTF8,
		ASCII
	};
	enum class FileAccess {
		Read,
		ReadWrite,
		Write
	};

	class File {

	public:
		static std::vector<std::string> ReadAllLines(const char* filename);
		static std::string ReadAllText(const char* filename);
		static bool Exists(const char* filename);

	private:
		static TextEncoding GetEncoding(ALLEGRO_FILE* file);

	};

}