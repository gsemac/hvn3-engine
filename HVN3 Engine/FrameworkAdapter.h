#pragma once
#include "Framework.h"
#ifdef HVN3_BACKEND_ALLEGRO5
#include <allegro5/allegro.h>

namespace hvn3 {

	class Color;
	class Font;
	
	namespace System {

		class FrameworkAdapter {

		public:
			static ALLEGRO_COLOR ToColor(const Color& color);
			static Color FromColor(const ALLEGRO_COLOR& color);
			static ALLEGRO_FONT* FromFont(const Font& font);

		};

	}
}

#endif