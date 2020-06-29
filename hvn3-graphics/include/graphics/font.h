#pragma once

#include "graphics/font_format.h"
#include "graphics/font_options.h"

#include <memory>
#include <string>

struct ALLEGRO_FONT;

namespace hvn3::graphics {

	class Font {

		using underlying_font_t = ALLEGRO_FONT;

	public:
		Font();
		Font(underlying_font_t* font, bool takeOwnership);
		Font(const Font& other);
		Font(Font&& other) noexcept;

		int Height() const;
		int Ascent() const;
		int Descent() const;
		int Size() const;

		underlying_font_t* GetUnderlyingData() const;

		Font& operator=(const Font& other);
		Font& operator=(Font&& other) noexcept;

		explicit operator bool() const;

		static Font FromBuffer(const uint8_t* buffer, size_t bufferSize, int fontSize, FontFormat fontFormat);
		static Font FromBuffer(const uint8_t* buffer, size_t bufferSize, int fontSize, FontFormat fontFormat, FontOptions options);
		static Font FromFile(const std::string& filePath, int fontSize);
		static Font FromFile(const std::string& filePath, int fontSize, FontOptions options);

		static Font Default();

	private:
		std::shared_ptr<underlying_font_t> font;
		int size;
		bool ownsFont;

		void CopyAssignFrom(const Font& other);
		void MoveAssignFrom(Font&& other);

		static int ConvertFontOptionsToFlags(FontOptions options);
		static void FreeFont(underlying_font_t* font);

	};

	bool operator==(const Font& lhs, const Font& rhs);
	bool operator!=(const Font& lhs, const Font& rhs);

}