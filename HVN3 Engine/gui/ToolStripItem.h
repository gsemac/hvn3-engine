#pragma once
#include "gui/Control.h"

namespace hvn3 {
	namespace Gui {
		
		enum class ToolStripItemDisplayStyle {
			None,
			Text,
			Image,
			ImageAndText
		};

		class ToolStripItem : public virtual Control {

		public:
			ToolStripItem();
			
			ToolStripItemDisplayStyle DisplayStyle() const;
			void SetDisplayStyle(ToolStripItemDisplayStyle style);

		private:
			ToolStripItemDisplayStyle _display_style;

		};

	}
}