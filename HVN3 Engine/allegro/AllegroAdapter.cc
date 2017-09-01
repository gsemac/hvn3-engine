#include "AllegroAdapter.h"
#include "Color.h"

namespace hvn3 {
	namespace System {

		ALLEGRO_COLOR AllegroAdapter::ToColor(const Color& color) {

			return al_map_rgba_f(color.Rf() * color.Alphaf(), color.Gf() * color.Alphaf(), color.Bf() * color.Alphaf(), color.Alphaf());

		}
		Color AllegroAdapter::FromColor(const ALLEGRO_COLOR& color) {

			return Color::FromArgbf(color.r, color.g, color.b, color.a);

		}
		ALLEGRO_FONT* AllegroAdapter::ToFont(const Font& font) {

			return font._font;

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

	}
}