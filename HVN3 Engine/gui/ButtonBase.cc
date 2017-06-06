#include "gui/ButtonBase.h"
#include "io/Mouse.h"
#include "Point.h"

namespace hvn3 {
	namespace Gui {

		ButtonBase::ButtonBase(const String& text) :
			TextableControl(text),
			_text_offset(0.0f, 0.0f) {
		}

		void ButtonBase::OnMouseEnter() {
			std::cout << FixedPosition();
			SetBackColor(BackColor().Lighter());
			Invalidate();

		}
		void ButtonBase::OnMouseLeave() {
			std::cout << "mouse leave!";
			SetBackColor(BackColor().Darker());

			if (Mouse::ButtonDown(MouseButton::Left))
				_text_offset.SetY(0.0f);

			Invalidate();

		}
		void ButtonBase::OnMouseDown() {

			bool invalidate_req = (_text_offset.Y() == 0.0f);
			_text_offset.SetY(1.0f);
			if (invalidate_req)
				Invalidate();

		}
		void ButtonBase::OnMouseUp() {

			_text_offset.SetY(0.0f);
			Invalidate();

		}
		void ButtonBase::OnClick() {

			if (Text().Contains(" Clicked!"))
				SetText(Text().SubString(0, Text().IndexOf(" Clicked!")));
			else
				SetText(Text() + " Clicked!");

		}
		void ButtonBase::OnPaint(PaintEventArgs& e) {

			// Draw background.
			e.Graphics().DrawFilledRectangle(0.0f, 0.0f, Width(), Height(), BackColor());

			// Draw text.
			if (Font()) {
				float tx = (std::round)((Width() / 2.0f) + _text_offset.X());
				float ty = (std::round)((Height() / 2.0f - Font()->Height() / 2.0f - 1.0f) + _text_offset.Y());
				e.Graphics().DrawText(tx + 1.0f, ty + 1.0f, Text(), Font(), Color::FromArgb(0, 0, 0, 0.5f), Alignment::Center);
				e.Graphics().DrawText(tx, ty, Text(), Font(), Color::FromArgb(186, 186, 186), Alignment::Center);
			}

			// Draw outline.
			e.Graphics().DrawRectangle(0.0f, 0.0f, Width(), Height(), Color::FromArgb(17, 17, 17), 1.0f);

		}

	}
}