#include "ToolStripSeparator.h"

namespace hvn3 {
	namespace Gui {

		ToolStripSeparator::ToolStripSeparator() :
			Control(Point(0, 0), Size(6, 25)) {

			_thickness = 2.0f;

		}

		void ToolStripSeparator::OnPaint(PaintEventArgs& e) {

			e.Graphics().Clear(BackColor());
			e.Graphics().DrawLine(Width() / 2.0f, 0.0f, Width() / 2.0f, Height(), BackColor().Lighter().Lighter(), _thickness);

		}

	}
}