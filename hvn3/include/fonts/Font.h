#pragma once
#include "utility/BitFlags.h"
#include <string>
#include <cstdint>

struct ALLEGRO_FONT;

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}

	enum class FontOptions {
		None = 1,
		Monochrome = 2,
		AutoScale = 4
	};
	ENABLE_BITFLAG_OPERATORS(FontOptions)

	enum class FontFormat {
		TrueTypeFont,
		Bitmap
	};

	class Font {
		friend class System::AllegroAdapter;

	public:
		Font();
		Font(const char* filename, int size, FontOptions options = FontOptions::None);
		Font(uint8_t* buffer, size_t buffer_size, int font_size, FontFormat format);
		Font(Font&& other);
		~Font();

		// Returns the height of the font in pixels.
		int Height() const;
		// Returns the font size that was passed in the constructor.
		int Size() const;

		// Creates and returns a built-in font resource.
		static Font BuiltIn();

		Font& operator=(Font&& other);

	protected:
		void AdjustScale(float scale_factor);
		bool AutoScaleEnabled() const;
		ALLEGRO_FONT* AlPtr() const;

	private:
		ALLEGRO_FONT* _font;
		bool _scalable;
		int _size;
		std::string _filename;
		FontOptions _flags;

	};

}