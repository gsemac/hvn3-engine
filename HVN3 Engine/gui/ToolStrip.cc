#include "ToolStrip.h"

namespace hvn3 {
	namespace Gui {

		ToolStrip::ToolStrip() :
			Control(Point(100, 0), Size(400, 25)) {
		}

		void ToolStrip::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());
			e.Graphics().DrawLine(0, Height(), Width(), Height(), BackColor().Darker(), 1);

		}

	}
}