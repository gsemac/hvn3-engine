#include "hvn3/gui/ButtonBase.h"
#include "hvn3/gui/_GuiManager.h"
#include "hvn3/io/Mouse.h"
#include "hvn3/math/Point2d.h"

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
		void ButtonBase::OnMouseDown(MouseEventArgs& e) {

			bool invalidate_req = (_text_offset.Y() == 0.0f);
			_text_offset.SetY(1.0f);
			if (invalidate_req)
				Invalidate();

		}
		void ButtonBase::OnMouseUp(MouseEventArgs& e) {

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

			_renderBase(e);
			_renderText(e);

		}

		// Protected methods

		void ButtonBase::_renderBase(PaintEventArgs& e) {

			// Draw background.
			e.Graphics().DrawSolidRectangle(0.0f, 0.0f, Width(), Height(), BackColor());

			// Draw outline.
			e.Graphics().DrawRectangle(0.0f, 0.0f, Width(), Height(), Color::FromArgb(17, 17, 17), 1.0f);

		}
		void ButtonBase::_renderText(PaintEventArgs& e) {

			// Draw text.
			if (Font()) {
				float tx = (std::round)((Width() / 2.0f) + _text_offset.X());
				float ty = (std::round)((Height() / 2.0f - Font()->Height() / 2.0f - 1.0f) + _text_offset.Y());
				e.Graphics().DrawText(tx + 1.0f, ty + 1.0f, Text(), Font(), Color::FromArgb(0, 0, 0, 128), Alignment::Center);
				e.Graphics().DrawText(tx, ty, Text(), Font(), Color::FromArgb(186, 186, 186), Alignment::Center);
			}

		}
		void ButtonBase::_renderImage(PaintEventArgs& e) {

			// Draw image.
			if (Image()) {
				float iy = Math::Round((Height() / 2.0f - Image()->Height() / 2.0f - 1.0f) + _text_offset.Y());
				e.Graphics().DrawBitmap(3, iy, Image());
			}

		}

	}
}