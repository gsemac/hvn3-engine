#include "Button.h"
#include <cmath>
#include "SizeableBase.h"
#include "UTF8String.h"
#include "Font.h"
#include "Graphics.h"
#include "io/Mouse.h"
#include "gui/ITextable.h"
#include "gui/GuiManager.h"

namespace hvn3 {

	namespace Gui {

		Button::Button(float x, float y, float width, float height, const String& text) :
			Control(PointF(x, y), SizeF(width, height)),
			ITextable(this, text),
			_text_offset(0.0f, 0.0f) {
		}

		void Button::OnMouseEnter() {

			SetBackColor(BackColor().Lighter());
			Invalidate();

			std::cout << "Mouse Enter!\n";

		}
		void Button::OnMouseLeave() {

			SetBackColor(BackColor().Darker());

			if (Mouse::ButtonDown(MouseButton::Left))
				_text_offset.SetY(0.0f);

			Invalidate();

			std::cout << "Mouse Leave!\n";

		}
		void Button::OnMouseDown(MouseEventArgs& e) {

			bool invalidate_req = (_text_offset.Y() == 0.0f);
			_text_offset.SetY(1.0f);
			if (invalidate_req)
				Invalidate();

			std::cout << "Mouse Down!\n";

		}
		void Button::OnMouseUp(MouseEventArgs& e) {

			_text_offset.SetY(0.0f);
			Invalidate();

			std::cout << "Mouse Up!\n";

		}
		void Button::OnClick() {

			if (Text().Contains(" Clicked!"))
				SetText(Text().SubString(0, Text().IndexOf(" Clicked!")));
			else
				SetText(Text() + " Clicked!");

			std::cout << Text() << ": Button Clicked!\n";

		}
		void Button::OnPaint(PaintEventArgs& e) {

			// Draw background.
			e.Graphics().DrawFilledRectangle(0.0f, 0.0f, Width(), Height(), BackColor());

			// Draw text.
			std::cout << Manager()->StyleManager() << std::endl;
			if (Font()) {
				float tx = (std::round)((Width() / 2.0f) + _text_offset.X());
				float ty = (std::round)((Height() / 2.0f - Font()->Height() / 2.0f - 1.0f) + _text_offset.Y());
				e.Graphics().DrawText(tx + 1.0f, ty + 1.0f, Text(), Font(), Color::FromArgb(0, 0, 0, 128), Alignment::Center);
				e.Graphics().DrawText(tx, ty, Text(), Font(), Color::FromArgb(186, 186, 186), Alignment::Center);
			}

			// Draw outline.
			e.Graphics().DrawRectangle(0.0f, 0.0f, Width(), Height(), Color::FromArgb(17, 17, 17), 1.0f);

		}

	}

}