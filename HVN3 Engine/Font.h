#pragma once
#include "BitFlags.h"
#include <string>

struct ALLEGRO_FONT;

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}

	enum class FontOptions {
		None = 1,
		Monochrome = 2,
		AutoScale = 3
	};
	ENABLE_BITFLAG_OPERATORS(FontOptions);

	class Font {
		friend class System::AllegroAdapter;

	public:
		Font(const char* filename, int size, FontOptions options = FontOptions::None);
		Font(Font&& other);
		~Font();

		// Returns the height of the font in pixels.
		int Height() const;
		// Returns the font size that was passed in the constructor.
		int Size() const;

		// Creates and returns a built-in font resource.
		static Font BuiltIn();

	protected:
		void AdjustScale(float scale_factor);
		bool AutoScaleEnabled() const;
		ALLEGRO_FONT* AlPtr() const;

	private:
		Font() = default;

		ALLEGRO_FONT* _font;
		bool _scalable;
		int _size;
		std::string _filename;
		FontOptions _flags;

	};

}