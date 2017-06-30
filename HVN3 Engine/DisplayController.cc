#include "DisplayController.h"
#include "Display.h"

namespace hvn3 {
	namespace System {

		DisplayController::DisplayController(Display* display) {

			_display = display;

		}

		void DisplayController::SetFocus(bool value) {
			
			_display->_SetFocus(value);

		}

	}
}