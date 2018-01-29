#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/io/MouseEventArgs.h"
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	namespace Gui {

		class IWidget;

		enum class WidgetEventType {
			// Generated when the widget is clicked.
			OnClick,
			OnMouseHover,
			// Generated when the widget changes position.
			OnMove
		};


		class WidgetEventArgs : public System::EventArgs {

		public:
			WidgetEventArgs(IWidget* sender);

			virtual IWidget* Sender() const;
			virtual WidgetEventType Type() const = 0;

		private:
			IWidget* _sender;

		};

		class WidgetMouseHoverEventArgs : public WidgetEventArgs {

		public:
			WidgetMouseHoverEventArgs(IWidget* sender, const PointF& position);

			const PointF& Position() const;
			WidgetEventType Type() const override;

		private:
			PointF _position;

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
		template <>
		struct GetWidgetEventType<WidgetEventType::OnMouseHover> {
			typedef WidgetMouseHoverEventArgs type;
		};
		template <>
		struct GetWidgetEventType<WidgetEventType::OnMove> {
			typedef WidgetMoveEventArgs type;
		};

		
	}
}