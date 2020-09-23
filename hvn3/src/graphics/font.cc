#include "graphics/font.h"

#include "core/engine.h"

#include <cassert>
#include <utility>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/allegro_ttf.h>

namespace hvn3::graphics {

	// Public members

	Font::Font() {

		size = 0;
		ownsFont = true;

	}
	Font::Font(underlying_font_t* font, bool takeOwnership) :
		Font() {

		if (font != nullptr) {

			if (takeOwnership) {

				core::Engine::Initialize(core::EngineModules::Core | core::EngineModules::Graphics);

				this->font = std::shared_ptr<underlying_font_t>(font, FreeFont);
				this->ownsFont = true;

			}
			else {

				this->font = std::shared_ptr<underlying_font_t>(std::shared_ptr<underlying_font_t>{}, font);
				this->ownsFont = false;

			}

		}

	}
	Font::Font(const Font& other) :
		Font() {

		CopyAssignFrom(other);

	}
	Font::Font(Font&& other) noexcept :
		Font() {

		MoveAssignFrom(std::move(other));

	}

	int Font::Height() const {

		return font ?
			al_get_font_line_height(GetUnderlyingData()) :
			0;

	}
	int Font::Ascent() const {

		return font ?
			al_get_font_ascent(GetUnderlyingData()) :
			0;

	}
	int Font::Descent() const {

		return font ?
			al_get_font_descent(GetUnderlyingData()) :
			0;

	}
	int Font::Size() const {

		return size;

	}

	Font::underlying_font_t* Font::GetUnderlyingData() const {

		return font.get();

	}

	Font& Font::operator=(const Font& other) {

		CopyAssignFrom(other);

		return *this;

	}
	Font& Font::operator=(Font&& other) noexcept {

		MoveAssignFrom(std::move(other));

		return *this;

	}

	Font::operator bool() const {

		return GetUnderlyingData() != nullptr;

	}

	Font Font::FromBuffer(const uint8_t* buffer, size_t bufferSize, int fontSize, FontFormat fontFormat) {

		return FromBuffer(buffer, bufferSize, fontSize, fontFormat, FontOptions::Default);

	}
	Font Font::FromBuffer(const uint8_t* buffer, size_t bufferSize, int fontSize, FontFormat fontFormat, FontOptions options) {

		if (buffer == nullptr || bufferSize <= 0)
			return Font();

		ALLEGRO_FILE* file = al_open_memfile((void*)buffer, bufferSize, "r");
		underlying_font_t* font = nullptr;

		assert(file != nullptr);

		switch (fontFormat) {

		case FontFormat::TrueType:

			font = al_load_ttf_font_f(file, "", fontSize, ConvertFontOptionsToFlags(options));

			break;

		}

		al_fclose(file);

		assert(font != nullptr);

		return Font(font, true);

	}
	Font Font::FromFile(const std::string& filePath, int fontSize) {

		return FromFile(filePath, fontSize, FontOptions::Default);

	}
	Font Font::FromFile(const std::string& filePath, int fontSize, FontOptions options) {

		underlying_font_t* font = al_load_font(filePath.c_str(), fontSize, ConvertFontOptionsToFlags(options));

		assert(font != nullptr);

		return Font(font, true);

	}


	Font Font::Default() {

		underlying_font_t* builtinFont = al_create_builtin_font();

		Font font = Font(builtinFont, true);

		font.size = 8;

		return font;

	}

	bool operator==(const Font& lhs, const Font& rhs) {

		return lhs.GetUnderlyingData() == rhs.GetUnderlyingData() &&
			lhs.Size() == rhs.Size();

	}
	bool operator!=(const Font& lhs, const Font& rhs) {

		return !(lhs == rhs);

	}

	// Private members

	void Font::CopyAssignFrom(const Font& other) {

		font = other.font;
		size = other.size;
		ownsFont = other.ownsFont;

	}
	void Font::MoveAssignFrom(Font&& other) {

		font = std::move(other.font);
		size = other.size;
		ownsFont = other.ownsFont;

		other.size = 0;

	}

	int Font::ConvertFontOptionsToFlags(FontOptions options) {

		int flags = 0;

		if (HasFlag(options, FontOptions::Monochrome))
			flags |= ALLEGRO_TTF_MONOCHROME;

		return flags;

	}
	void Font::FreeFont(underlying_font_t* font) {

		if (font != nullptr) {

			al_destroy_font(font);

			core::Engine::Deinitialize(core::EngineModules::Core | core::EngineModules::Graphics);

		}

	}

}