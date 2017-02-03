#include "EventArgs.h"
#include "Exception.h"
#include "allegro5\allegro.h"

EventArgs::EventArgs(const EventSource& source) {

	//__source = source;
	__timestamp = al_get_time();

}
const EventSource& EventArgs::Source() const {

	throw NotImplementedException();

	//return __source;

}
