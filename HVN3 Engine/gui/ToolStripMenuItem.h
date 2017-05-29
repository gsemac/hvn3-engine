#pragma once
#include "gui/TextableControl.h"

namespace hvn3 {
	namespace Gui {

		class ToolStripMenuItem : public TextableControl {

		public:
			ToolStripMenuItem();

			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			

		};

	}
}