#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/sound/Sound.h"
#include <allegro5/allegro_ttf.h>
#define ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS 4096

namespace hvn3 {
	namespace System {

		ALLEGRO_COLOR AllegroAdapter::ToColor(const Color& color) {

			return al_map_rgba_f(color.Rf() * color.Alphaf(), color.Gf() * color.Alphaf(), color.Bf() * color.Alphaf(), color.Alphaf());

		}
		Color AllegroAdapter::FromColor(const ALLEGRO_COLOR& color) {

			return Color::FromArgbf(color.r, color.g, color.b, color.a);

		}
		ALLEGRO_FONT* AllegroAdapter::ToFont(const Font& font) {

			return font.AlPtr();

		}
		int AllegroAdapter::ToFontFlags(FontFlags flags) {

			int al_flags = 0;

			if (HasFlag(flags, FontFlags::Monochrome))
				al_flags |= ALLEGRO_TTF_MONOCHROME;

			return al_flags;

		}
		ALLEGRO_BITMAP* AllegroAdapter::ToBitmap(const Graphics::Bitmap& bitmap) {

			return bitmap._get_bitmap_ptr();

		}
		int AllegroAdapter::ToBitmapFlags(Graphics::BitmapFlags flags) {

			int al_flags = 0;

			if (HasFlag(flags, Graphics::BitmapFlags::AllegroDefault) || HasFlag(flags, Graphics::BitmapFlags::Default))
				al_flags |= ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS;

			if (HasFlag(flags, Graphics::BitmapFlags::MinLinear))
				al_flags |= ALLEGRO_MIN_LINEAR;

			if (HasFlag(flags, Graphics::BitmapFlags::MagLinear))
				al_flags |= ALLEGRO_MAG_LINEAR;

			return al_flags;

		}
		ALLEGRO_DISPLAY* AllegroAdapter::ToDisplay(const Display& display) {
			return display.get();
		}
		Graphics::BitmapFlags AllegroAdapter::FromBitmapFlags(int flags) {

			int al_flags = flags;
			int my_flags = 0;

			if (al_flags & ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS)
				my_flags |= ALLEGRO_DEFAULT_NEW_BITMAP_FLAGS;

			if (al_flags & ALLEGRO_MIN_LINEAR)
				my_flags |= static_cast<int>(Graphics::BitmapFlags::MinLinear);

			if (al_flags & ALLEGRO_MAG_LINEAR)
				my_flags |= static_cast<int>(Graphics::BitmapFlags::MagLinear);

			return static_cast<Graphics::BitmapFlags>(my_flags);

		}
		int AllegroAdapter::ToDisplayFlags(DisplayFlags flags) {

			int allegro_flags = 0;

			if (HasFlag(flags, DisplayFlags::OpenGL))
				allegro_flags |= ALLEGRO_OPENGL;
			if (HasFlag(flags, DisplayFlags::Direct3D))
				allegro_flags |= ALLEGRO_DIRECT3D_INTERNAL;
			if (HasFlag(flags, DisplayFlags::Resizable))
				allegro_flags |= ALLEGRO_RESIZABLE;
			if (HasFlag(flags, DisplayFlags::NoBorder))
				allegro_flags |= ALLEGRO_FRAMELESS;
			if (HasFlag(flags, DisplayFlags::OpenGL3))
				allegro_flags |= ALLEGRO_OPENGL_3_0;
			if (HasFlag(flags, DisplayFlags::OpenGLForwardCompatible))
				allegro_flags |= ALLEGRO_OPENGL_FORWARD_COMPATIBLE;
			if (HasFlag(flags, DisplayFlags::FullscreenWindow))
				allegro_flags |= ALLEGRO_FULLSCREEN_WINDOW;

			return allegro_flags;

		}
		int AllegroAdapter::ToDisplayOption(DisplayOption option) {

			switch (option) {
			case DisplayOption::VsyncEnabled:
				return ALLEGRO_VSYNC;
			}

			return 0;

		}
		int AllegroAdapter::ToAlignmentFlags(Alignment flags) {

			int al_flags = 0;

			if (HasFlag(flags, Alignment::Left))
				al_flags |= ALLEGRO_ALIGN_LEFT;
			if (HasFlag(flags, Alignment::Right))
				al_flags |= ALLEGRO_ALIGN_RIGHT;
			if (HasFlag(flags, Alignment::Center))
				al_flags |= ALLEGRO_ALIGN_CENTER;

			return al_flags;


		}
		ALLEGRO_SAMPLE* AllegroAdapter::ToSample(const Sound& sound) {
			return sound._getPtr();
		}
		ALLEGRO_USTR* AllegroAdapter::ToUStr(const String& input) {
			return input._getUstrPointer();
		}

	}
}