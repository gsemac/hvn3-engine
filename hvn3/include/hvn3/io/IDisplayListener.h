#pragma once

#include "hvn3/events/EventArgs.h"
#include "hvn3/io/Display.h"

namespace hvn3 {

	class IDisplayListener {

	public:
		class DisplayEventArgs : public hvn3::EventArgs {
		public:
			DisplayEventArgs(Display* display) {
				_display = display;
			}
			const Display& Display() const {
				return *_display;
			}
		private:
			hvn3::Display* _display;
		};

		class DisplaySizeChangedEventArgs : public DisplayEventArgs {
		public:
			using DisplayEventArgs::DisplayEventArgs;
			const SizeI& NewSize() const {
				return this->Display().Size();
			}
		};

		typedef DisplayEventArgs DisplayClosedEventArgs;
		typedef DisplayEventArgs DisplayLostEventArgs;
		typedef DisplayEventArgs DisplayFoundEventArgs;

		virtual void OnDisplayClosed(DisplayClosedEventArgs& e) = 0;
		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) = 0;
		virtual void OnDisplayLost(DisplayLostEventArgs& e) = 0;
		virtual void OnDisplayFound(DisplayFoundEventArgs& e) = 0;

	};

}