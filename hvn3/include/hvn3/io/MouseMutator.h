#pragma once
#include "hvn3/io/MouseEventArgs.h"

namespace hvn3 {
	namespace System {

		class MouseMutator {

		public:
			MouseMutator() = default;

			void ResetButtonStates(bool pressed, bool released, bool held) const;
			void SetButtonState(MouseButton button, bool pressed) const;
			void SetScrollState(bool scrolled_up, bool scrolled_down) const;
			void SetPosition(float x, float y) const;
			void SetDisplayPosition(int x, int y) const;

			void DispatchAllMouseDownEvents() const;

			void DispatchEvent(MouseDownEventArgs& e) const;
			void DispatchEvent(MousePressedEventArgs& e) const;
			void DispatchEvent(MouseReleasedEventArgs& e) const;
			void DispatchEvent(MouseMoveEventArgs& e) const;
			void DispatchEvent(MouseScrollEventArgs& e) const;

			EventSource GetEventSource() const;

		};

	}
}