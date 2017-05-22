#include <iostream>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "Font.h"

namespace hvn3 {

	Font::Font(const char* filename, int size, FontOptions options) {

		__flags = options;
		__font = al_load_font(filename, size, (HasFlag(options, FontOptions::Monochrome)) ? ALLEGRO_TTF_MONOCHROME : NULL);
		__scalable = HasFlag(options, FontOptions::Scalable);
		__filename = filename;
		__size = size;
		__height = al_get_font_line_height(__font);

	}
	Font::Font(Font&& other) {

		__font = other.__font;
		__scalable = other.__scalable;
		__size = other.__size;
		__height = other.__height;
		__filename = other.__filename;
		__flags = other.__flags;

		other.__font = nullptr;
		other.__scalable = false;
		other.__size = 0;
		other.__height = 0;
		other.__filename = nullptr;
		other.__flags = FontOptions::None;

	}
	Font::~Font() {

		if (__font)
			al_destroy_font(__font);
		__font = nullptr;

	}

	void Font::Scale(float scale_factor) {

		ALLEGRO_FONT* scaled_font = al_load_font(__filename, __size * scale_factor, (HasFlag(__flags, FontOptions::Monochrome)) ? ALLEGRO_TTF_MONOCHROME : NULL);

		if (scaled_font) {
			ALLEGRO_FONT* old_font = __font;
			__font = scaled_font;
			al_destroy_font(old_font);
			__height = al_get_font_line_height(__font);
		}

	}
	bool Font::IsScalable() const {

		return __scalable;

	}
	int Font::Height() const {

		return __height;

	}

	ALLEGRO_FONT* Font::AlPtr() const {

		return __font;

	}

	Font::Font() {}
	Font Font::BuiltIn() {

		Font f;

		f.__font = al_create_builtin_font();
		f.__scalable = false;
		f.__size = 8.0f;
		f.__height = al_get_font_line_height(f.__font);
		f.__filename = "";
		f.__flags = FontOptions::Monochrome;

		return f;

	}

}