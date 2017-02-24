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
#include "AllegroExtensions.h"
#include "ITextable.h"

namespace GUI {

	class Button : public Control, public ITextable {

	private:
		Utf8String __text;
		Font* __font;

		Point __text_offset;

	public:
		Button(float x, float y, float width, float height, const char* text) :
			Control(Point(x, y), Size(width, height)),
			ITextable(text),
			__text_offset(0.0f, 0.0f) {

			__font = GUI::ActiveTheme()->DefaultFont();

		}

		virtual void SetText(const char* text) override {

			ITextable::SetText(text);
			Invalidate();

		}
		virtual void SetText(const std::string& text) override {

			ITextable::SetText(text);
			Invalidate();

		}
		virtual void SetText(const Utf8String& text) override {

			ITextable::SetText(text);
			Invalidate();

		}

		virtual void OnMouseEnter() {

			SetBackColor(BackColor().Lighter());
			Invalidate();

			std::cout << "Mouse Enter!\n";

		}
		virtual void OnMouseLeave() {

			SetBackColor(BackColor().Darker());

			if (Mouse::ButtonDown(MB_LEFT))
				__text_offset.SetY(0.0f);

			Invalidate();

			std::cout << "Mouse Leave!\n";

		}
		virtual void OnMouseDown() {

			bool invalidate_req = (__text_offset.Y() == 0.0f);
			__text_offset.SetY(1.0f);
			if (invalidate_req)
				Invalidate();

			std::cout << "Mouse Down!\n";

		}
		virtual void OnMouseUp() {

			__text_offset.SetY(0.0f);
			Invalidate();

			std::cout << "Mouse Up!\n";

		}
		virtual void OnClick() {

			if (Text().Contains(" Clicked!"))
				SetText(Text().Substring(0, Text().IndexOf(" Clicked!")));
			else
				SetText(Text() + " Clicked!");

			std::cout << "Button Clicked!\n";

		}
		virtual void OnPaint(PaintEventArgs& e) override {
			
			// Draw background.
			e.Graphics().DrawFilledRectangle(0.0f, 0.0f, Width(), Height(), BackColor());

			// Draw text.
			if (__font) {
				float tx = (std::round)((Width() / 2.0f) + __text_offset.X());
				float ty = (std::round)((Height() / 2.0f - __font->Height() / 2.0f - 1.0f) + __text_offset.Y());
				e.Graphics().DrawText(tx + 1.0f, ty + 1.0f, Text(), *__font, Color::FromArgb(0, 0, 0, 0.5f), Alignment::Center);
				e.Graphics().DrawText(tx, ty, Text(), *__font, Color::FromArgb(186, 186, 186), Alignment::Center);
			}

			// Draw outline.
			e.Graphics().DrawRectangle(0.0f, 0.0f, Width(), Height(), Color::FromArgb(17, 17, 17), 1.0f);

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