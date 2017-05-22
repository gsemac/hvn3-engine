#pragma once
#include "Stopwatch.h"
#include "Geometry.h"
#include "BitFlags.h"
#include "EventSource.h"

namespace hvn3 {

	enum MOUSE_BUTTONS {
		MB_LEFT = 0x01,
		MB_MIDDLE = 0x02,
		MB_RIGHT = 0x04
	};
	ENABLE_BITFLAG_OPERATORS(MOUSE_BUTTONS);

	enum class SystemCursor {
		None = 0,
		Default = 1,
		Arrow = 2,
		Busy = 3,
		Question = 4,
		Edit = 5,
		Move = 6,
		ResizeN = 7,
		ResizeW = 8,
		ResizeS = 9,
		ResizeE = 10,
		ResizeNW = 11,
		ResizeSW = 12,
		ResizeSE = 13,
		ResizeNE = 14,
		Progress = 15,
		Precision = 16,
		Link = 17,
		AltSelect = 18,
		Unavailable = 19
	};

	class Mouse {
		friend class StateAccessor;

	public:
		class StateAccessor {

		public:
			static void ResetButtonStates(bool pressed, bool released, bool held);
			static void SetButtonState(MOUSE_BUTTONS button, bool pressed);
			static void SetScrollState(bool scrolled_up, bool scrolled_down);
			static void SetPosition(float x, float y);
			static void SetDisplayPosition(int x, int y);

		};

		static float X, Y;

		static bool ButtonDown(MOUSE_BUTTONS);
		static bool ButtonPressed(MOUSE_BUTTONS);
		static bool ButtonReleased(MOUSE_BUTTONS);
		static bool ButtonDoubleClicked(MOUSE_BUTTONS);
		static bool ScrolledUp();
		static bool ScrolledDown();
		static bool ScrolledLeft();
		static bool ScrolledRight();
		static bool InRegion(Rectangle rect);
		static bool InRegion(float x1, float y1, float x2, float y2);
		static Point Position();
		static Point GlobalPosition();
		static const Point& DisplayPosition();
		static void ShowCursor();
		static void HideCursor();
		static void SetCursor(SystemCursor cursor);
		static EventSource EventSource();

	private:
		struct MouseButton {

			bool held, pressed, dbl_clicked, locked, released;
			bool dbl_waiting, dbl_waiting_allowed;
			Stopwatch last_release; // for double-click detection

			MouseButton();

		};
		static Point __last_click_pos; // for double-click detection (mouse can't move)
		static Point __display_mouse_position; // Mouse position relative to the display

		static MouseButton __left, __middle, __right;
		static bool __scrolled_up, __scrolled_down, _scrolled_left, _scrolled_right;

		Mouse();
		static MouseButton* ToMouseButton(MOUSE_BUTTONS);

	};

}