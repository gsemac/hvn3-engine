#pragma once
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/events/EventArgs.h"
#include "hvn3/gui2/GuiTypeDefs.h"
#include "hvn3/io/MouseEventArgs.h"
#include "hvn3/math/Point2d.h"

// Specializes the widget event type traits class for accessing a widget event type from a widget event ID.
#define HVN3_DECLARE_WIDGET_EVENT_TYPE(ID, TYPE) template <> struct hvn3::Gui::WidgetEventTypeTraits<ID> { typedef TYPE type; };

namespace hvn3 {

	namespace Gui {

		class IWidget;
		class TabStripItem;
		class WidgetManager;

		// Base class for widget event args classes. The Type() method must be overridden to return the event ID.
		class IWidgetEventArgs : public System::EventArgs {
		public:
			virtual IWidget* Sender() const = 0;
			virtual WidgetEventType Type() const = 0;
		};

		template <WidgetEventType EVENT_TYPE>
		class WidgetEventArgsBase : public IWidgetEventArgs {
		public:
			WidgetEventArgsBase(IWidget* sender) :
				_sender(sender) {
			}
			IWidget* Sender() const override {
				return _sender;
			}
			WidgetEventType Type() const override {
				return EVENT_TYPE;
			}
		private:
			IWidget* _sender;
		};

		class WidgetUpdateEventArgs : public WidgetEventArgsBase<WidgetEventType::OnUpdate> {
		public:
			WidgetUpdateEventArgs(IWidget* sender, double dt) :
				WidgetEventArgsBase(sender),
				_dt(dt) {
			}
			double Delta() const {
				return _dt;
			}
		private:
			double _dt;
		};

		template <WidgetEventType EVENT_TYPE>
		class WidgetMouseEventArgsBase : public WidgetEventArgsBase<EVENT_TYPE> {
		public:
			WidgetMouseEventArgsBase(IWidget* sender, MouseEventArgs& e) :
				WidgetEventArgsBase(sender),
				_args(e) {
			}
			MouseButton Button() const {
				return _args.Button();
			}
			const PointF& Position() const {
				return _args.Position();
			}
		private:
			MouseEventArgs _args;
		};

		class WidgetMouseHoverEventArgs : public WidgetUpdateEventArgs {
		public:
			WidgetMouseHoverEventArgs(IWidget* sender, const PointF& position, double dt) :
				WidgetUpdateEventArgs(sender, dt),
				_position(position) {
			}
			const PointF& Position() const {
				return _position;
			}
			WidgetEventType Type() const override {
				return WidgetEventType::OnMouseHover;
			}
		private:
			PointF _position;
		};

		template <WidgetEventType EVENT_TYPE>
		class WidgetMouseMoveEventArgsBase : public WidgetEventArgsBase<EVENT_TYPE> {
		public:
			WidgetMouseMoveEventArgsBase(IWidget* sender, MouseMoveEventArgs& e) :
				WidgetEventArgsBase(sender),
				_position(e.Position()) {
			}
			const PointF& Position() const {
				return _position;
			}
		private:
			PointF _position;
		};

		class WidgetMoveEventArgs : public WidgetEventArgsBase<WidgetEventType::OnMove> {
		public:
			WidgetMoveEventArgs(IWidget* sender, const PointF& old_position, const PointF& new_position) :
				WidgetEventArgsBase(sender),
				_old_position(old_position),
				_new_position(new_position) {
			}
			const PointF& OldPosition() const {
				return _old_position;
			}
			const PointF& NewPosition() const {
				return _new_position;
			}
		private:
			PointF _old_position;
			PointF _new_position;
		};

		class WidgetDrawEventArgs : public WidgetEventArgsBase<WidgetEventType::OnDraw> {
		public:
			WidgetDrawEventArgs(IWidget* sender, DrawEventArgs* e) :
				WidgetEventArgsBase(sender),
				_args(e) {
			}
			Graphics::Graphics& Graphics() {
				return _args->Graphics();
			}
		private:
			DrawEventArgs* _args;
		};

		class WidgetManagerChangedEventArgs : public WidgetEventArgsBase<WidgetEventType::OnManagerChanged> {
		public:
			WidgetManagerChangedEventArgs(IWidget* sender, WidgetManager* previous_manager) :
				WidgetEventArgsBase(sender),
				_previous_manager(previous_manager) {
			}
			WidgetManager* OldManager() {
				return _previous_manager;
			}
		private:
			WidgetManager* _previous_manager;
		};

		class ChildWidgetAddedEventArgs : public WidgetEventArgsBase<WidgetEventType::OnChildWidgetAdded> {
		public:
			ChildWidgetAddedEventArgs(IWidget* sender, IWidget* child) :
				WidgetEventArgsBase(sender),
				_child(child) {
			}
			IWidget* Child() {
				return _child;
			}
		private:
			IWidget* _child;
		};

		class WidgetSelectedTabChangedEventArgs : public WidgetEventArgsBase<WidgetEventType::OnSelectedTabChanged> {
		public:
			WidgetSelectedTabChangedEventArgs(IWidget* sender, TabStripItem* tab) :
				WidgetEventArgsBase(sender),
				_child(tab) {
			}
			TabStripItem* Tab() {
				return _child;
			}
		private:
			TabStripItem* _child;
		};

		typedef WidgetMouseEventArgsBase<WidgetEventType::OnMouseReleased> WidgetMouseReleasedEventArgs;
		typedef WidgetMouseEventArgsBase<WidgetEventType::OnMouseDown> WidgetMouseDownEventArgs;
		typedef WidgetMouseEventArgsBase<WidgetEventType::OnMousePressed> WidgetMousePressedEventArgs;
		typedef WidgetMouseEventArgsBase<WidgetEventType::OnMouseClick> WidgetMouseClickEventArgs;
		typedef WidgetMouseMoveEventArgsBase<WidgetEventType::OnMouseMove> WidgetMouseMoveEventArgs;
		typedef WidgetMouseMoveEventArgsBase<WidgetEventType::OnMouseEnter> WidgetMouseEnterEventArgs;
		typedef WidgetMouseMoveEventArgsBase<WidgetEventType::OnMouseLeave> WidgetMouseLeaveEventArgs;
		typedef WidgetEventArgsBase<WidgetEventType::OnRendererChanged> WidgetRendererChangedEventArgs;
		typedef WidgetEventArgsBase<WidgetEventType::OnFocus> WidgetFocusEventArgs;
		typedef WidgetEventArgsBase<WidgetEventType::OnFocusLost> WidgetFocusLostEventArgs;
		typedef WidgetEventArgsBase<WidgetEventType::OnZDepthChanged> WidgetZDepthChangedEventArgs;

		template <WidgetEventType WIDGET_EVENT_TYPE>
		struct WidgetEventTypeTraits {
			typedef IWidgetEventArgs type;
		};

		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnUpdate, WidgetUpdateEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseClick, WidgetMouseClickEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseDown, WidgetMouseDownEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseEnter, WidgetMouseEnterEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseHover, WidgetMouseHoverEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseLeave, WidgetMouseLeaveEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseReleased, WidgetMouseReleasedEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMousePressed, WidgetMousePressedEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseMove, WidgetMouseMoveEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMove, WidgetMoveEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnFocus, WidgetFocusEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnFocusLost, WidgetFocusLostEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnDraw, WidgetDrawEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnZDepthChanged, WidgetZDepthChangedEventArgs)
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnChildWidgetAdded, ChildWidgetAddedEventArgs)

	}
}