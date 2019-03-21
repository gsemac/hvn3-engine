#pragma once

#include "hvn3/io/IODefs.h"
#include "hvn3/math/Point2d.h"
#include "hvn3/utility/Stopwatch.h"

namespace hvn3 {

	class MouseState {

		struct MouseButtonState {

			MouseButtonState();

			bool held, pressed, dbl_clicked, locked, released;
			bool dbl_waiting, dbl_waiting_allowed;
			Stopwatch last_release; // for double-click detection

		};

	public:
		class Iterator {

		public:
			class MouseButtonStateWrapper {

			public:
				MouseButtonStateWrapper() = default;
				MouseButtonStateWrapper(MouseButton button, MouseButtonState state);

				bool Held() const;
				bool Pressed() const;
				bool Released() const;
				MouseButton Button() const;

			private:
				MouseButton _button;
				MouseButtonState _state;

			};

			Iterator(MouseState* state, MouseButton button);

			bool operator==(const Iterator& other);
			bool operator!=(const Iterator& other);
			Iterator& operator++();
			Iterator operator++(int);
			MouseButtonStateWrapper& operator*();
			MouseButtonStateWrapper* operator->();
			const MouseButtonStateWrapper& operator*() const;
			const MouseButtonStateWrapper* operator->() const;

		private:
			void _initMouseButtonState() const;
			void _increment();

			MouseState* _state;
			MouseButton _button;
			mutable MouseButtonStateWrapper _mouse_button_state;

		};

		MouseState();

		bool ButtonDown(MouseButton button);
		bool ButtonPressed(MouseButton button);
		bool ButtonReleased(MouseButton button);
		bool ButtonDoubleClicked(MouseButton button);
		bool ScrolledUp();
		bool ScrolledDown();
		bool ScrolledLeft();
		bool ScrolledRight();
		PointF Position();
		PointF ScreenPosition();
		const PointF& DisplayPosition();

		void ClearButtonStates();
		void ClearButtonStates(bool pressed, bool released, bool held);
		void SetButtonState(MouseButton button, bool pressed);
		void SetScrollState(bool scrolledUp, bool scrolledDown);
		void SetPosition(float x, float y);
		void SetDisplayPosition(int x, int y);

		Iterator begin();
		Iterator end();

	private:
		MouseButtonState* _toMouseButtonState(hvn3::MouseButton button);

		float x, y;
		PointF _last_click_position; // For double-click detection (double-click invalidated on movement)
		PointF _display_position; // Mouse position relative to the display
		MouseButtonState _left, _middle, _right;
		bool _scrolled_up, _scrolled_down, _scrolled_left, _scrolled_right;

	};

}