#include "GUITheme.h"

GUI::GUITheme::GUITheme() {}
GUI::GUITheme* GUI::GUITheme::__active_theme = nullptr;

GUI::GUITheme* GUI::ActiveTheme() {

	if (!GUI::GUITheme::__active_theme)
		GUI::GUITheme::__active_theme = new GUI::DefaultTheme();
	
	return GUI::GUITheme::__active_theme;

}

// Themes: DefaultTheme
unsigned int GUI::DefaultTheme::__ref_count = 0;
Font* GUI::DefaultTheme::__font_ptr = nullptr;
Sprite* GUI::DefaultTheme::__exit_icon_ptr = nullptr;
