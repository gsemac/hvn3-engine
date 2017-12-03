#include "hvn3/gui/ToolStripSeparator.h"
#include "hvn3/gui/GuiManager.h"

namespace hvn3 {
	namespace Gui {

		ToolStripSeparator::ToolStripSeparator() :
			Control(PointF(0, 0), SizeF(6, 25)) {

			_thickness = 1.0f;

		}

		void ToolStripSeparator::OnPaint(PaintEventArgs& e) {

			// Draw background.
			//e.Graphics().DrawFilledRectangle(0, 0, Width(), Height(), BackColor());

			e.Graphics().DrawLine(Width() / 2.0f, 2.0f, Width() / 2.0f, Height() - 2.0f, Manager()->StyleManager()->SecondaryColor(), 1);

		}

	}
}