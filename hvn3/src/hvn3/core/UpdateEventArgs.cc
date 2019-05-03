#include "hvn3/core/UpdateEventArgs.h"

namespace hvn3 {

	UpdateEventArgs::UpdateEventArgs(const ApplicationContext& context, double deltaTime) :
		EventArgs(context),
		_delta_time(deltaTime) {
	}

	double UpdateEventArgs::Delta() const {
		return _delta_time;
	}

}