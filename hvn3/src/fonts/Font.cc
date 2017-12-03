#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_memfile.h>
#include "fonts/Font.h"
#include "exceptions/Exception.h"
#include <cassert>

namespace hvn3 {

	// Public methods

	Font::Font() {

		_flags = FontOptions::None;
		_font = nullptr;
		_scalable = false;
		_size = 0;

	}
	Font::Font(const char* filename, int size, FontOptions options) {

		_flags = options;
		_font = al_load_font(filename, size, (HasFlag(options, FontOptions::Monochrome)) ? ALLEGRO_TTF_MONOCHROME : NULL);
		_scalable = HasFlag(options, FontOptions::AutoScale);
		_filename = filename;
		_size = size;

	}
	Font::Font(uint8_t* buffer, size_t buffer_size, int font_size, FontFormat format) :
		Font() {

		if (buffer == nullptr || buffer_size <= 0)
			return;

		ALLEGRO_FILE* file = al_open_memfile(buffer, buffer_size, "r");

		assert(file != nullptr);

		switch (format) {

		case FontFormat::TrueTypeFont:
			_font = al_load_ttf_font_f(file, "", font_size, NULL);
			break;

		default:
			al_fclose(file);
			throw System::NotImplementedException();

		}

		al_fclose(file);

	}
	Font::Font(Font&& other) {

		*this = std::move(other);

	}
	Font::~Font() {

		// Free the memory used by the font object.
		if (_font)
			al_destroy_font(_font);
		_font = nullptr;

	}

	int Font::Height() const {

		if (_font != nullptr)
			return al_get_font_line_height(_font);
		else
			return 0;

	}
	int Font::Size() const {

		return _size;

	}

	Font Font::BuiltIn() {

		Font font;

		font._font = al_create_builtin_font();
		font._scalable = false;
		font._size = 8.0f;
		font._flags = FontOptions::Monochrome;

		return font;

	}

	Font& Font::operator=(Font&& other) {

		_font = other._font;
		_scalable = other._scalable;
		_size = other._size;
		_filename = other._filename;
		_flags = other._flags;

		other._font = nullptr;
		other._scalable = false;
		other._size = 0;
		other._filename = "";
		other._flags = FontOptions::None;

		return *this;

	}

	// Protected methods

	void Font::AdjustScale(float scale_factor) {

		ALLEGRO_FONT* scaled_font = al_load_font(_filename.c_str(), _size * scale_factor, (HasFlag(_flags, FontOptions::Monochrome)) ? ALLEGRO_TTF_MONOCHROME : NULL);

		if (scaled_font) {
			ALLEGRO_FONT* old_font = _font;
			_font = scaled_font;
			al_destroy_font(old_font);
		}

	}
	bool Font::AutoScaleEnabled() const {

		return _scalable;

	}

	ALLEGRO_FONT* Font::AlPtr() const {

		return _font;

	}

}