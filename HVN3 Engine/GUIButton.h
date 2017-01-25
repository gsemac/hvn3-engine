#ifndef __GUIBUTTON_H
#define __GUIBUTTON_H
#include <allegro5\allegro_primitives.h>
#include <cmath>
#include "ISizeable.h"
#include "UTF8String.h"
#include "Control.h"
#include "Font.h"
#include "Graphics.h"
#include "GUITheme.h"
#include "Mouse.h"

namespace Gui {

	class Button : public Control {

	private:
		Utf8String* __text;
		Font* __font;

		Point __text_offset;
		
	public:
		Button(float x, float y, float width, float height, const char* text) : Control(Point(x, y), Size(width, height)), __text_offset(0.0f, 0.0f) {

			__font = Gui::ActiveTheme()->DefaultFont();
			__text = new Utf8String(text);

		}

		Utf8String* Text() {

			return __text;

		}
		void SetText(const char* text) {

			if (__text)
				delete __text;

			__text = new Utf8String(text);

			Invalidate();

		}

		virtual void OnMouseEnter() {

			BackColor()->Lighten();
			Invalidate();

		}
		virtual void OnMouseLeave() {

			BackColor()->Darken();

			if (Mouse::ButtonDown(MB_LEFT))
				__text_offset.Y() = 0.0f;

			Invalidate();

		}
		virtual void OnMouseDown() {

			bool invalidate_req = (__text_offset.Y() == 0.0f);
			__text_offset.Y() = 1.0f;
			if (invalidate_req)
				Invalidate();

		}
		virtual void OnMouseUp() {

			__text_offset.Y() = 0.0f;
			Invalidate();

		}
		virtual void OnClick() {

			std::cout << "Button: Clicked!\n";


			SetText("Clicked!");

		}
		virtual void OnPaint() {

			// Draw background.
			al_draw_filled_rectangle(X, Y, X + Width(), Y + Height(), BackColor()->AlPtr());

			// Draw text.
			if (__font)
				al_draw_shadow_ustr(__font->AlPtr(), al_map_rgb(186, 186, 186), Color::FromArgb(0, 0, 0, 0.5f).AlPtr(), (std::round)((X + Width() / 2.0f) + __text_offset.X),
				(std::round)((Y + Height() / 2.0f - __font->Height() / 2.0f - 1) + __text_offset.Y), ALLEGRO_ALIGN_CENTRE, __text->AlPtr());

			// Draw outline.
			al_draw_rectangle(X + 1, Y + 1, X + Width(), Y + Height(), al_map_rgb(17, 17, 17), 1);

		}

	};

	class Button1 : public Button {

	private:
		float __min_brightness, __max_brightness;

	public:
		Button1(float x, float y, float width, float height, const char* text) : Button(x, y, width, height, text) {

			__min_brightness = 0.0f; //__primary_color.Luminance();
			__max_brightness = __min_brightness + 0.2f;

		}
		//void OnMouseLeave() {

		//	std::cout << "Mouse left! (" << Mouse::X << ")\n";
		//	//__primary_color.Darken();

		//	//__primary_color = Gui::ActiveTheme()->PrimaryColor();

		//}
		//void OnMouseEnter() {

		//	std::cout << "Mouse entered! (" << Mouse::X << ")\n";
		//	//__primary_color.Lighten();

		//}
		//void OnMouseHover() {

		//	// Increase luminance.
		//	/*if (__primary_color.Luminance() < __max_brightness)
		//	__primary_color.Lighten(Properties::FPS / 10000.0F);*/

		//}
		//void Update() {

		//	Button::Update();

		//	//if (!Mouse::InRegion(X, Y, X + Width(), Y + Height()))


		//}

	};

}

#endif