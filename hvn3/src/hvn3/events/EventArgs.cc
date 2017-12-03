#include "hvn3/events/EventArgs.h"
#include "hvn3/exceptions/Exception.h"
#include <allegro5/allegro.h>

namespace hvn3 {
	namespace System {

		EventArgs::EventArgs() {

			_timestamp = al_get_time();

		}
		EventArgs::EventArgs(const EventSource& source) : EventArgs() {

			//__source = source;


		}
		const EventSource& EventArgs::Source() const {

			throw System::NotImplementedException();

			//return __source;

		}
		double EventArgs::Timestamp() const {

			return _timestamp;

		}
		bool EventArgs::Handled() const {

			return _handled;

		}
		void EventArgs::SetHandled(bool value) {

			_handled = value;

		}

	}
}