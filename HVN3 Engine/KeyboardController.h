#pragma once
#include "KeyEventArgs.h"

namespace hvn3 {
	namespace System {

		class KeyboardController {

		public:
			KeyboardController() = default;

			void ResetKeyStates(bool pressed, bool released, bool held) const;
			void ResetKeyStates() const;
			void SetKeyState(int key_code, bool pressed) const;
			void SetLastChar(int key_char) const;
			
			void DispatchAllKeyDownEvents() const;

			void DispatchEvent(KeyDownEventArgs& e) const;
			void DispatchEvent(KeyPressedEventArgs& e) const;
			void DispatchEvent(KeyUpEventArgs& e) const;
			void DispatchEvent(KeyCharEventArgs& e) const;
			
			System::EventSource GetEventSource() const;

		};

	}
}