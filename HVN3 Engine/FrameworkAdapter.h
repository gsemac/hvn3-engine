#pragma once
#include "Framework.h"
#ifdef HVN3_BACKEND_ALLEGRO5
#include <allegro5/allegro.h>
#include "Color.h"

namespace hvn3 {
	namespace System {

		class FrameworkAdapter {

		public:
			static ALLEGRO_COLOR ToColor(const Color& color);
			static Color FromColor(const ALLEGRO_COLOR& color);

		};

	}
}

#endif