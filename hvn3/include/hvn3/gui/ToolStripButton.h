#pragma once
#include "hvn3/gui/ButtonBase.h"
#include "hvn3/gui/ToolStripItem.h"

namespace hvn3 {
	namespace Gui {

		class ToolStripButton : public ToolStripItem, public ButtonBase {

		public:
			ToolStripButton();

			virtual void OnPaint(PaintEventArgs& e) override;

		};

	}
}