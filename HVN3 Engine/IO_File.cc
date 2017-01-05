#include "IO_File.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>
#include "Exception.h"

namespace IO {

	// Public methods

	std::vector<std::string> File::ReadAllLines(const char* filename) {

		// Open the file.
		if (!Exists(filename)) throw IO::FileNotFoundException();
		ALLEGRO_FILE* file = al_fopen(filename, "r");

		// Determine the encoding, and seek past the BOM bytes if needed.
		TextEncoding encoding = GetEncoding(file);
		if (encoding == TextEncoding::UTF8) al_fseek(file, 3, ALLEGRO_SEEK_SET);

		// Initialize lines vector.
		std::vector<std::string> lines;

		// Read the file bytes.
		std::string line;
		while (!al_feof(file)) {

			char next_byte = al_fgetc(file);

			if (line.size() > 0 && line[line.size() - 1] == 0x0D && next_byte == 0x0A) { // Newline: 0D 0A
				line.pop_back();
				lines.push_back(line);
				line.clear();
			}
			else
				line.push_back(next_byte);

		};
		// Push the last line on if it isn't empty.
		if (!line.empty()) lines.push_back(line);

		// Return the result.
		return lines;

	}
	std::string File::ReadAllText(const char* filename) {

		// Open the file.
		if (!Exists(filename)) throw IO::FileNotFoundException();
		ALLEGRO_FILE* file = al_fopen(filename, "r");

		// Determine the encoding, and seek past the BOM bytes if needed.
		TextEncoding encoding = GetEncoding(file);
		if (encoding == TextEncoding::UTF8) al_fseek(file, 3, ALLEGRO_SEEK_SET);

		// Initialize the string to hold the file contents.
		std::string file_contents;

		// Read the file bytes.
		while (!al_feof(file)) {
			char next_byte = al_fgetc(file);
			file_contents.push_back(next_byte);
		};

		// Return the result.
		return file_contents;

	}
	bool File::Exists(const char* filename) {

		return al_filename_exists(filename);

	}

	// Private methods

	TextEncoding File::GetEncoding(ALLEGRO_FILE* file) {

		// The default encoding is ASCII.
		TextEncoding encoding = TextEncoding::ASCII;

		// Read the first three bytes of the file to read the BOM.
		unsigned char bom[3];
		size_t i;
		for (i = 0; i < 3 && !al_feof(file); i++)
			bom[i] = al_fgetc(file);
		if (i >= 3 && bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)
			encoding = TextEncoding::UTF8;

		// Seek back to the beginning of the file.
		al_fseek(file, 0, ALLEGRO_SEEK_SET);

		// Return the encoding.
		return encoding;

	}

}