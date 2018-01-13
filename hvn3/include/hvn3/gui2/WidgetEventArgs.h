#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/math/Point2d.h"

namespace hvn3 {
	namespace Gui {

		enum class GuiEvent {
			// Generated when the widget is clicked.
			OnClick,
			// Generated when the widget changes position.
			OnMove
		};


		class WidgetEventArgs : public System::EventArgs {

		public:
			virtual GuiEvent Type() const = 0;
			
		};
		

		class MoveEventArgs : public WidgetEventArgs {

		public:
			MoveEventArgs(const PointF& old_position, const PointF& new_position);

			const PointF& OldPosition() const;
			const PointF& NewPosition() const;
			GuiEvent Type() const;

		private:
			PointF _old_position;
			PointF _new_position;

		};

	}
}