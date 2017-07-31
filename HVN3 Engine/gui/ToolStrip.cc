#include "ToolStrip.h"
#define DEFAULT_LEFT_PADDING 4.0f
#define DEFAULT_TOP_PADDING 2.0f
#define DEFAULT_BOTTOM_PADDING 1.0f

namespace hvn3 {
	namespace Gui {

		ToolStrip::ToolStrip() :
			Control(PointF(0, 0), SizeF(400, 25)) {

			SetDock(DockStyle::Top);

		}

		void ToolStrip::AddItem(ToolStripItem* item) {

			item->SetParent(this);

			if (_controls()->ControlCount() > 0) {
				auto last_control = _controls()->ControlAt(_controls()->ControlCount() - 1);
				item->SetPosition(last_control->X() + last_control->Width(), DEFAULT_TOP_PADDING);
			}
			else
				item->SetPosition(DEFAULT_LEFT_PADDING, DEFAULT_TOP_PADDING);

			item->SetHeight(Height() - DEFAULT_TOP_PADDING - DEFAULT_BOTTOM_PADDING);

			_controls()->AddControl(ControlPtr(item));

		}

		void ToolStrip::OnPaint(PaintEventArgs& e) {

			// Draw the control's background.
			e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());

			// Draw shine.
			e.Graphics().DrawRectangle(1, 1, Width(), Height(), Manager()->StyleManager()->SecondaryColor().Lighter(), 1);

			// Draw child controls.			
			ContainerControl::OnPaint(e);

			// Draw outline.
			e.Graphics().DrawRectangle(0, 0, Width(), Height(), Manager()->StyleManager()->SecondaryColor(), 1);

			//e.Graphics().DrawLine(0, Height(), Width(), Height(), BackColor().Darker(), 1);

		}

	}
}