#pragma once
#include "utility/Stopwatch.h"
#include "math/GeometryUtils.h"
#include "utility/BitFlags.h"
#include "events/EventSource.h"

namespace hvn3 {

	namespace System {
		class MouseController;
	}

	enum class MouseButton {
		Left = 0x01,
		Middle = 0x02,
		Right = 0x04
	};
	ENABLE_BITFLAG_OPERATORS(MouseButton);

	enum class MouseScrollDirection {
		Up = 1,
		Down = 2,
		Left = 4,
		Right = 8
	};
	ENABLE_BITFLAG_OPERATORS(MouseScrollDirection);

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
		friend class System::MouseController;

	public:
		static float X, Y;

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
		static void SetCursor(SystemCursor cursor);

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