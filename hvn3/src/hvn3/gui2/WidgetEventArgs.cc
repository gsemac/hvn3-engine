#include "hvn3/gui2/WidgetEventArgs.h"


namespace hvn3 {
	namespace Gui {

		WidgetEventArgs::WidgetEventArgs(IWidget* sender) {

			_sender = sender;

		}
		IWidget* WidgetEventArgs::Sender() const {

			return _sender;

		}


		WidgetUpdateEventArgs::WidgetUpdateEventArgs(IWidget* sender, double dt) :
			WidgetEventArgs(sender),
			_dt(dt) {
		}
		double WidgetUpdateEventArgs::Delta() const {
			return _dt;
		}
		WidgetEventType WidgetUpdateEventArgs::Type() const {
			return WidgetEventType::OnUpdate;
		}

		WidgetMouseEventArgs::WidgetMouseEventArgs(IWidget* sender, WidgetEventType type, MouseEventArgs& e) :
			WidgetEventArgs(sender),
			_args(e),
			_type(type) {
		}
		MouseButton WidgetMouseEventArgs::Button() const {
			return _args.Button();
		}
		const PointF& WidgetMouseEventArgs::Position() const {
			return _args.Position();
		}
		WidgetEventType WidgetMouseEventArgs::Type() const {
			return _type;
		}

		WidgetMouseHoverEventArgs::WidgetMouseHoverEventArgs(IWidget* sender, const PointF& position, double dt) :
			WidgetUpdateEventArgs(sender, dt),
			_position(position) {
		}
		const PointF& WidgetMouseHoverEventArgs::Position() const {

			return _position;

		}
		WidgetEventType WidgetMouseHoverEventArgs::Type() const {
			return WidgetEventType::OnMouseHover;
		}

		WidgetMouseMoveEventArgs::WidgetMouseMoveEventArgs(IWidget* sender, WidgetEventType type, MouseMoveEventArgs& e) :
			WidgetEventArgs(sender), _type(type), _position(e.Position()) {
		}
		const PointF& WidgetMouseMoveEventArgs::Position() const {
			return _position;
		}
		WidgetEventType WidgetMouseMoveEventArgs::Type() const {
			return _type;
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