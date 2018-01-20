#include "hvn3/gui2/WidgetEventArgs.h"


namespace hvn3 {
	namespace Gui {

		MouseHoverEventArgs::MouseHoverEventArgs(MouseEventArgs& e) :
			_position(e.Position()) {
		}
		const PointF& MouseHoverEventArgs::Position() const {

			return _position;

		}
		WidgetEvent MouseHoverEventArgs::Type() const {

			return WidgetEvent::OnMouseHover;

		}

		MoveEventArgs::MoveEventArgs(const PointF& old_position, const PointF& new_position) :
			_old_position(old_position),
			_new_position(new_position) {
		}
		const PointF& MoveEventArgs::OldPosition() const {

			return _old_position;

		}
		const PointF& MoveEventArgs::NewPosition() const {

			return _new_position;

		}
		WidgetEvent MoveEventArgs::Type() const {

			return WidgetEvent::OnMove;

		}

	}
}