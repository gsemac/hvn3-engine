#include "ToolStrip.h"

namespace hvn3 {
	namespace Gui {

		ToolStrip::ToolStrip() :
			Control(PointF(0, 0), SizeF(400, 25)) {

			SetDock(DockStyle::Top);

		}

		void ToolStrip::AddItem(ToolStripItem* item) {

			item->SetParent(this);

			if (_Controls()->ControlCount() > 0) {
				auto last_control = _Controls()->ControlAt(_Controls()->ControlCount() - 1);
				item->SetPosition(last_control->X() + last_control->Width(), 0);
			}
			else
				item->SetPosition(0, 0);

			item->SetHeight(Height());

			_Controls()->AddControl(Control::Create(item));

		}

		void ToolStrip::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());
			
			ContainerControl::OnPaint(e);

			e.Graphics().DrawLine(0, Height(), Width(), Height(), BackColor().Darker(), 1);

		}

	}
}