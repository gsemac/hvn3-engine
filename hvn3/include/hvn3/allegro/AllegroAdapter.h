#pragma once
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/io/Display.h"
#include "hvn3/utility/Utf8String.h"
#include <allegro5/color.h>

namespace hvn3 {

	class Color;
	class Font;
	class Sound;

	namespace System {

		class AllegroAdapter {

		public:
			// Creates an ALLEGRO_COLOR from a Color instance.
			static ALLEGRO_COLOR ToColor(const Color& color);
			// Creates a Color from an ALLEGRO_COLOR instance.
			static Color FromColor(const ALLEGRO_COLOR& color);
			// Returns a pointer to the underlying ALLEGRO_FONT from a font instance.
			static ALLEGRO_FONT* ToFont(const Font& font);
			// Converts font flags to Allegro font flags.
			static int ToFontFlags(FontFlags flags);
			// Returns a pointer to the underlying ALLEGRO_BITMAP from a bitmap instance.
			static ALLEGRO_BITMAP* ToBitmap(const Graphics::Bitmap& bitmap);
			// Converts bitmap flags to Allegro bitmap flags.
			static int ToBitmapFlags(Graphics::BitmapFlags flags);
			// Converts Allegro bitmap flags to bitmap flags.
			static Graphics::BitmapFlags FromBitmapFlags(int flags);
			// Returns a pointer to the underlying ALLEGRO_DISPLAY from a Display instance.
			static ALLEGRO_DISPLAY* ToDisplay(const Display& display);
			// Converts display flags to Allegro display flags.
			static int ToDisplayFlags(DisplayFlags flags);
			// Converts display option to Allegro display option.
			static int ToDisplayOption(DisplayOption option);
			static int ToAlignmentFlags(Alignment flags);
			static ALLEGRO_SAMPLE* ToSample(const Sound& sound);
			static ALLEGRO_USTR* ToUStr(const String& input);

		private:
			// It should not be possible to instantiate this class.
			AllegroAdapter() = default;

		};

	}
}