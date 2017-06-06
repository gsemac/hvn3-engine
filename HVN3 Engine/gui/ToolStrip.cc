#include "ToolStrip.h"

namespace hvn3 {
	namespace Gui {

		ToolStrip::ToolStrip() :
			Control(Point(0, 0), Size(400, 25)) {

			SetDock(DockStyle::Top);

		}

		void ToolStrip::AddItem(ToolStripItem* item) {

			item->SetParent(this);

			item->SetHeight(Height());

			_Controls()->AddControl(Control::Create(item));

		}

		void ToolStrip::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());
			e.Graphics().DrawLine(0, Height(), Width(), Height(), BackColor().Darker(), 1);

			ContainerControl::OnPaint(e);

		}

	}
}