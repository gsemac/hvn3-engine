#include "graphics/DisplayController.h"
#include "graphics/Display.h"

namespace hvn3 {
	namespace System {

		DisplayController::DisplayController(Display* display) {

			_display = display;

		}

		void DisplayController::SetFocus(bool value) {
			
			_display->_setFocus(value);

		}

	}
}