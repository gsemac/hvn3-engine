#pragma once
#include "gui/ButtonBase.h"
#include "gui/ToolStripItem.h"

namespace hvn3 {
	namespace Gui {

		class ToolStripButton : public ToolStripItem, public ButtonBase {

		public:
			ToolStripButton();

			virtual void OnPaint(PaintEventArgs& e) override;

		};

	}
}