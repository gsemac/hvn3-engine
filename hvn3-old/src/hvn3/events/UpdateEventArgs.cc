#include "hvn3/events/UpdateEventArgs.h"

namespace hvn3 {

	UpdateEventArgs::UpdateEventArgs(double deltaTime) :
		_delta_time(deltaTime) {
	}

	double UpdateEventArgs::Delta() const {
		return _delta_time;
	}

}