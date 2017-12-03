#include "hvn3/io/Clipboard.h"
#include "hvn3/utility/UTF8String.h"
#include "hvn3/graphics/Display.h"
#include <allegro5\allegro.h>

namespace hvn3 {

	void Clipboard::SetText(const char* text) {

		Display* active_display = Display::ActiveDisplay();
		if (active_display)
			al_set_clipboard_text(active_display->AlPtr(), text);

	}
	void Clipboard::SetText(const std::string& text) {

		Display* active_display = Display::ActiveDisplay();
		if (active_display)
			al_set_clipboard_text(active_display->AlPtr(), text.c_str());

	}
	void Clipboard::SetText(const String& text) {

		Display* active_display = Display::ActiveDisplay();
		if (active_display)
			al_set_clipboard_text(active_display->AlPtr(), text.c_str());

	}
	std::string Clipboard::GetText() {

		// Get a pointer to the active Display.
		Display* active_display = Display::ActiveDisplay();

		// Get text from the clipboard if there is an active Display.
		char* text = nullptr;
		if (active_display)
			text = al_get_clipboard_text(active_display->AlPtr());

		if (text) {

			std::string str(text);
			al_free(text);
			return str;

		}
		else
			return std::string("");

	}
	bool Clipboard::HasText() {

		Display* active_display = Display::ActiveDisplay();
		if (active_display)
			return  al_clipboard_has_text(active_display->AlPtr());
		else
			return false;

	}

}