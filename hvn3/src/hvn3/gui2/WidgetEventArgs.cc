#include "hvn3/gui2/WidgetEventArgs.h"


namespace hvn3 {
	namespace Gui {

		WidgetEventArgs::WidgetEventArgs(IWidget* sender) {

			_sender = sender;

		}
		IWidget* WidgetEventArgs::Sender() const {

			return _sender;

		}

		WidgetMouseHoverEventArgs::WidgetMouseHoverEventArgs(IWidget* sender, const PointF& position) :
			WidgetEventArgs(sender),
			_position(position) {
		}
		const PointF& WidgetMouseHoverEventArgs::Position() const {

			return _position;

		}
		WidgetEventType WidgetMouseHoverEventArgs::Type() const {

			return WidgetEventType::OnMouseHover;

		}

		WidgetMoveEventArgs::WidgetMoveEventArgs(IWidget* sender, const PointF& old_position, const PointF& new_position) :
			WidgetEventArgs(sender),
			_old_position(old_position),
			_new_position(new_position) {
		}
		const PointF& WidgetMoveEventArgs::OldPosition() const {

			return _old_position;

		}
		const PointF& WidgetMoveEventArgs::NewPosition() const {

			return _new_position;

		}
		WidgetEventType WidgetMoveEventArgs::Type() const {

			return WidgetEventType::OnMove;

		}

	}
}