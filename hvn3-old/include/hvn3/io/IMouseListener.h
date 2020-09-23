#pragma once

#include "hvn3/events/EventArgs.h"
#include "hvn3/io/IODefs.h"
#include "hvn3/math/Point2d.h"

namespace hvn3 {

	class IMouseListener {

	public:
		class MouseEventArgs :
			public EventArgs {

		public:
			MouseEventArgs(MouseButton button, const PointF& displayPosition, const PointF& worldPosition);

			MouseButton Button() const;
			const PointF& Position() const;
			float X() const;
			float Y() const;
			const PointF& DisplayPosition() const;

		private:
			MouseButton _button;
			PointF _position;
			PointF _display_position;

		};

		class MousePressedEventArgs :
			public MouseEventArgs {

		public:
			MousePressedEventArgs(MouseButton button, const PointF& displayPosition, const PointF& worldPosition, int clicks);

			int Clicks() const;

		private:
			int _clicks;

		};

		class MouseScrollEventArgs :
			public MouseEventArgs {

		public:
			MouseScrollEventArgs(const PointF& displayPosition, const PointF& worldPosition, const PointI& scrollPosition, int scrollDeltaX, int scrollDeltaY);

			const PointI& ScrollPosition() const;
			int ScrollX() const;
			int ScrollY() const;
			int DeltaX() const;
			int DeltaY() const;
			MouseScrollDirection Direction() const;

		private:
			MouseScrollDirection _direction;
			PointI _scroll_position;
			int _x_delta;
			int _y_delta;

		};

		class MouseMoveEventArgs :
			public MouseEventArgs {

		public:
			MouseMoveEventArgs(const PointF& displayPosition, const PointF& worldPosition);

		};

		class MouseDownEventArgs :
			public MouseEventArgs {
		public:
			using MouseEventArgs::MouseEventArgs;
		};

		class MouseReleasedEventArgs :
			public MouseEventArgs {
		public:
			using MouseEventArgs::MouseEventArgs;
		};

		class MouseLostEventArgs : public EventArgs {};
		class MouseFoundEventArgs : public EventArgs {};

		virtual void OnMouseDown(MouseDownEventArgs& e) = 0;
		virtual void OnMousePressed(MousePressedEventArgs& e) = 0;
		virtual void OnMouseReleased(MouseReleasedEventArgs& e) = 0;
		virtual void OnMouseMove(MouseMoveEventArgs& e) = 0;
		virtual void OnMouseScroll(MouseScrollEventArgs& e) = 0;

	};

}