#pragma once
#include "gui/ToolStripItem.h"

namespace hvn3 {
	namespace Gui {

		class ToolStripSeparator : public ToolStripItem {

		public:
			ToolStripSeparator();

			virtual void OnPaint(PaintEventArgs& e) override;

		private:
			float _thickness;

		};

	}
}