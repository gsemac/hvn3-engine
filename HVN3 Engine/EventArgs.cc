#include "EventArgs.h"
#include "Exception.h"
#include "allegro5\allegro.h"

namespace hvn3 {

	EventArgs::EventArgs() {

		__timestamp = al_get_time();

	}
	EventArgs::EventArgs(const EventSource& source) : EventArgs() {

		//__source = source;


	}
	const EventSource& EventArgs::Source() const {

		throw System::NotImplementedException();

		//return __source;

	}
	double EventArgs::Timestamp() const {

		return __timestamp;

	}

}