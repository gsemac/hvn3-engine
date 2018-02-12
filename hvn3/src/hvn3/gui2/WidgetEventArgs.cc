#include "hvn3/gui2/WidgetEventArgs.h"


namespace hvn3 {
	namespace Gui {

		WidgetEventArgs::WidgetEventArgs(IWidget* sender) {

			_sender = sender;

		}
		IWidget* WidgetEventArgs::Sender() const {

			return _sender;

		}


		WidgetUpdateEventArgs::WidgetUpdateEventArgs(IWidget* sender, float dt) :
			WidgetEventArgs(sender),
			_dt(dt) {
		}
		float WidgetUpdateEventArgs::Delta() const {
			return _dt;
		}
		WidgetEventType WidgetUpdateEventArgs::Type() const {
			return WidgetEventType::OnUpdate;
		}


		WidgetMouseHoverEventArgs::WidgetMouseHoverEventArgs(IWidget* sender, const PointF& position, float dt) :
			WidgetUpdateEventArgs(sender, dt),
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