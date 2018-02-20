#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/io/MouseEventArgs.h"
#include "hvn3/math/Point2d.h"
#define HVN3_DECLARE_WIDGET_EVENT_TYPE(ID, TYPE)\
template <>\
struct hvn3::Gui::GetWidgetEventType<ID> {\
	typedef TYPE type;\
};\

namespace hvn3 {

	namespace Gui {

		class IWidget;

		enum class WidgetEventType {
			// Generated when the widget is clicked.
			OnMouseClick,
			OnMouseDown,
			OnMouseEnter,
			OnMouseHover,
			OnMouseLeave,
			OnMouseUp,
			// Generated when the widget changes position.
			OnMove,
			OnUpdate
		};


		class WidgetEventArgs : public System::EventArgs {

		public:
			WidgetEventArgs(IWidget* sender);

			virtual IWidget* Sender() const;
			virtual WidgetEventType Type() const = 0;

		private:
			IWidget* _sender;

		};


		class WidgetUpdateEventArgs : public WidgetEventArgs {
		public:
			WidgetUpdateEventArgs(IWidget* sender, float dt);
			float Delta() const;
			WidgetEventType Type() const override;
		private:
			float _dt;
		};


		class WidgetMouseEventArgs : public WidgetEventArgs {
		public:
			WidgetMouseEventArgs(IWidget* sender, WidgetEventType type, MouseEventArgs& e);
			MouseButton Button() const;
			const PointF& Position() const;
			WidgetEventType Type() const override;
		private:
			MouseEventArgs _args;
			WidgetEventType _type;
		};
		class WidgetMouseHoverEventArgs : public WidgetUpdateEventArgs {
		public:
			WidgetMouseHoverEventArgs(IWidget* sender, const PointF& position, float dt);
			const PointF& Position() const;
			WidgetEventType Type() const override;
		private:
			PointF _position;
		};
		class WidgetMouseMoveEventArgs : public WidgetEventArgs {
		public:
			WidgetMouseMoveEventArgs(IWidget* sender, WidgetEventType type, MouseMoveEventArgs& e);
			const PointF& Position() const;
			WidgetEventType Type() const override;
		private:
			PointF _position;
			WidgetEventType _type;
		};

		class WidgetMoveEventArgs : public WidgetEventArgs {
		public:
			WidgetMoveEventArgs(IWidget* sender, const PointF& old_position, const PointF& new_position);
			const PointF& OldPosition() const;
			const PointF& NewPosition() const;
			WidgetEventType Type() const override;
		private:
			PointF _old_position;
			PointF _new_position;
		};

		template <WidgetEventType WIDGET_EVENT_TYPE>
		struct GetWidgetEventType {
			typedef WidgetEventArgs type;
		};

		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnUpdate, WidgetUpdateEventArgs);
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseDown, WidgetMouseEventArgs);
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseEnter, WidgetMouseEventArgs);
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseHover, WidgetMouseHoverEventArgs);
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseLeave, WidgetMouseEventArgs);
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMouseUp, WidgetMouseEventArgs);
		HVN3_DECLARE_WIDGET_EVENT_TYPE(WidgetEventType::OnMove, WidgetMoveEventArgs);

	}
}