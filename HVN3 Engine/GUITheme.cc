#include "GUITheme.h"

Gui::GUITheme::GUITheme() {}
Gui::GUITheme* Gui::GUITheme::__active_theme = nullptr;

Gui::GUITheme* Gui::ActiveTheme() {

	if (!Gui::GUITheme::__active_theme)
		Gui::GUITheme::__active_theme = new Gui::DefaultTheme();
	
	return Gui::GUITheme::__active_theme;

}

// Themes: DefaultTheme
unsigned int Gui::DefaultTheme::__ref_count = 0;
Font* Gui::DefaultTheme::__font_ptr = nullptr;
Sprite* Gui::DefaultTheme::__exit_icon_ptr = nullptr;
