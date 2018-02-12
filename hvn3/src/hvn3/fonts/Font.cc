#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_memfile.h>
#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/exceptions/Exception.h"
#include <cassert>

namespace hvn3 {

	Font::Font() {

		_flags = static_cast<FontFlags>(0);
		_font = font_ptr_type();
		_size = 0;

	}
	Font::Font(const std::string& filename, int size, FontFlags flags) {

		_flags = flags;
		_font = font_ptr_type(al_load_font(filename.c_str(), size, System::AllegroAdapter::ToFontFlags(flags)), al_destroy_font);
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
			_font = font_ptr_type(al_load_ttf_font_f(file, "", font_size, NULL), al_destroy_font);
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

	int Font::Height() const {

		if (_font != nullptr)
			return al_get_font_line_height(AlPtr());
		else
			return 0;

	}
	int Font::Size() const {

		return _size;

	}

	Font Font::BuiltIn() {

		Font font;

		font._font = font_ptr_type(al_create_builtin_font(), al_destroy_font);
		font._size = 8.0f;
		font._flags = FontFlags::Monochrome;

		return font;

	}

	Font& Font::operator=(Font&& other) {

		_font = std::move(other._font);
		_size = other._size;
		_filename = other._filename;
		_flags = other._flags;

		other._size = 0;
		other._filename = "";
		other._flags = static_cast<FontFlags>(0);

		return *this;

	}

	Font::operator bool() const {
		return static_cast<bool>(_font);
	}



	void Font::AdjustScale(float scale_factor) {

		ALLEGRO_FONT* scaled_font = al_load_font(_filename.c_str(), _size * scale_factor, System::AllegroAdapter::ToFontFlags(_flags));

		if (scaled_font != nullptr)
			_font = font_ptr_type(scaled_font, al_destroy_font);

	}
	bool Font::AutoScaleEnabled() const {

		return HasFlag(_flags, FontFlags::Monochrome);

	}

	ALLEGRO_FONT* Font::AlPtr() const {

		return _font.get();

	}

}