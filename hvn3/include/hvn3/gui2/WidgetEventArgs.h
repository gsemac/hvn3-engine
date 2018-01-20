#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/io/MouseEventArgs.h"
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	namespace Gui {

		enum class WidgetEvent {
			// Generated when the widget is clicked.
			OnClick,
			OnMouseHover,
			// Generated when the widget changes position.
			OnMove
		};


		class WidgetEventArgs : public System::EventArgs {

		public:
			virtual WidgetEvent Type() const = 0;

		};

		class MouseHoverEventArgs : public WidgetEventArgs {

		public:
			MouseHoverEventArgs(MouseEventArgs& e);

			const PointF& Position() const;
			WidgetEvent Type() const;

		private:
			PointF _position;

		};

		class MoveEventArgs : public WidgetEventArgs {

		public:
			MoveEventArgs(const PointF& old_position, const PointF& new_position);

			const PointF& OldPosition() const;
			const PointF& NewPosition() const;
			WidgetEvent Type() const;

		private:
			PointF _old_position;
			PointF _new_position;

		};

	}
}