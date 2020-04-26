#pragma once

#include "hvn3/events/EventSource.h"
#include "hvn3/io/IMouseListener.h"
#include "hvn3/io/IODefs.h"
#include "hvn3/io/ListenerBase.h"
#include "hvn3/io/system_cursor.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/utility/BitFlags.h"
#include "hvn3/utility/Stopwatch.h"

namespace hvn3 {

	namespace System {
		class MouseMutator;
	}

	class Mouse {
		friend class System::MouseMutator;

	public:
		typedef ListenerCollection<IMouseListener> Listeners;

		static float x, y;

		static bool ButtonDown(MouseButton button);
		static bool ButtonPressed(MouseButton button);
		static bool ButtonReleased(MouseButton button);
		static bool ButtonDoubleClicked(MouseButton button);
		static bool ScrolledUp();
		static bool ScrolledDown();
		static bool ScrolledLeft();
		static bool ScrolledRight();
		static bool InRegion(const RectangleF& region);
		static bool InRegion(float x1, float y1, float x2, float y2);
		static PointF Position();
		static PointF GlobalPosition();
		static const PointF& DisplayPosition();
		static void ShowCursor();
		static void HideCursor();
		static void SetCursor(io::SystemCursor cursor);
		static EventSource GetEventSource();

	private:
		struct MouseButton {

			bool held, pressed, dbl_clicked, locked, released;
			bool dbl_waiting, dbl_waiting_allowed;
			Stopwatch last_release; // for double-click detection

			MouseButton();

		};

		Mouse(); // A mouse object cannot be instantiated
		static MouseButton* ToMouseButton(hvn3::MouseButton button);

		static PointF _last_click_position; // For double-click detection (mouse can't move)
		static PointF _display_position; // Mouse position relative to the display
		static MouseButton _left, _middle, _right;
		static bool _scrolled_up, _scrolled_down, _scrolled_left, _scrolled_right;

	};

}