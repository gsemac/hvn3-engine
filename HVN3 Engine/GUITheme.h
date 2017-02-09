#ifndef __GUITHEME_H
#define __GUITHEME_H
#include "Font.h"
#include "Sprite.h"
#include "Color.h"

namespace GUI {

	class GUITheme {
		static GUITheme* __active_theme;
		friend GUITheme* ActiveTheme();

	public:
		GUITheme();
		virtual Font* DefaultFont() = 0;
		virtual Color PrimaryColor() = 0;
		virtual Color SecondaryColor() = 0;
		virtual Color HighlightColor() = 0;
		virtual Sprite* ExitIcon() {
		
			return nullptr;

		}

	};

	// Returns the GUI theme currently in use.
	GUITheme* ActiveTheme();

	// The default GUI appearance.
	class DefaultTheme : public GUITheme {

	private:
		static unsigned int __ref_count;
		static Font* __font_ptr;
		static Sprite* __exit_icon_ptr;

	public:
		DefaultTheme() {

			// Increase reference count.
			++__ref_count;

			// Initialize resources (if they haven't already been initialized).
			if (__ref_count == 1) {

				__font_ptr = new Font("sys/fonts/webly.ttf", 11, FontOptions::Monochrome);
				__exit_icon_ptr = new Sprite("sys/icons/close.png");

			}

		}
		~DefaultTheme() {

			// Decrease reference count.
			--__ref_count;

			// If reference count is 0, free resources.
			if (__ref_count == 0) {

				delete __font_ptr;
				delete __exit_icon_ptr;

			}

		}
		Font* DefaultFont() override  {

			return __font_ptr;

		}
		Color PrimaryColor() override  {

			return Color::FromArgb(35, 35, 35);

		}
		Color SecondaryColor() override {

			return Color::FromArgb(186, 186, 186);

		}
		Color HighlightColor() override {

			return Color(27, 161, 226);

		}
		Sprite* ExitIcon() override  {

			return __exit_icon_ptr;

		}

	};

}

#endif