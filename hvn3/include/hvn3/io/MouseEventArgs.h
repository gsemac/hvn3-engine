#pragma once
#include "hvn3/events/EventArgs.h"
#include "hvn3/io/Mouse.h"

namespace hvn3 {

	class MouseEventArgs : public System::EventArgs {

	public:
		MouseEventArgs(MouseButton button);

		MouseButton Button() const;
		const PointF& Position() const;

	private:
		MouseButton _button;
		PointF _position;

	};

	class MouseDownEventArgs : public MouseEventArgs {

	public:
		using MouseEventArgs::MouseEventArgs;
		
	};

	class MousePressedEventArgs : public MouseEventArgs {

	public:
		MousePressedEventArgs(MouseButton button, int clicks);
		
		int Clicks() const;

	private:
		int _clicks;

	};

	class MouseReleasedEventArgs : public MouseEventArgs {

	public:
		using MouseEventArgs::MouseEventArgs;

	};

	class MouseScrollEventArgs : public System::EventArgs {

	public:
		MouseScrollEventArgs(int x, int y, int x_delta, int y_delta);
		
		int ScrollX() const;
		int ScrollY() const;
		int DeltaX() const;
		int DeltaY() const;
		MouseScrollDirection Direction() const;
		const PointF& Position() const;

	private:
		MouseScrollDirection _direction;
		int _x;
		int _y;
		int _x_delta;
		int _y_delta;
		PointF _position;

	};

	class MouseMoveEventArgs : public System::EventArgs {

	public:
		MouseMoveEventArgs();

		const PointF& Position() const;

	private:
		PointF _position;

	};
	
}