#pragma once

struct ALLEGRO_DISPLAY;

namespace hvn3 {

	class Display;

	namespace System {

		class DisplayController {

		public:
			DisplayController(Display* display);

			// Sets a flag indicating whether or not the display currently has focus.
			void SetFocus(bool value);

		private:
			Display* _display;

		};

	}
}