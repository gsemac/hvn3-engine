#pragma once
#include "hvn3/utility/BitFlags.h"
#include <string>
#include <cstdint>
#include <memory>

struct ALLEGRO_FONT;

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}

	enum class FontFlags {
		// Loads the font with no anti-aliasing.
		Monochrome = 2,
		AutoScale = 4
	};
	ENABLE_BITFLAG_OPERATORS(FontFlags)

	enum class FontFormat {
		TrueTypeFont,
		Bitmap
	};

	class Font {
		friend class System::AllegroAdapter;

		typedef std::shared_ptr<ALLEGRO_FONT> font_shared_ptr_type;

	public:
		Font();
		// Instantiates a font from a file.
		Font(const std::string& filename, int size, FontFlags flags = static_cast<FontFlags>(0));
		// Instantiates a font from a memory buffer.
		Font(uint8_t* buffer, size_t buffer_size, int font_size, FontFormat format);

		Font(Font&& other);
		Font(const Font& other) = default;

		// Returns the height of the font in pixels.
		int Height() const;
		// Returns the font size that was passed in the constructor.
		int Size() const;

		// Instantiates the built-in font resource.
		static Font BuiltIn();

		Font& operator=(Font&& other);
		Font& operator=(const Font& other);
		explicit operator bool() const;

	protected:
		void AdjustScale(float scale_factor);
		bool AutoScaleEnabled() const;
		ALLEGRO_FONT* AlPtr() const;

	private:
		font_shared_ptr_type _font;
		std::string _filename;
		int _size;
		FontFlags _flags;

	};

}