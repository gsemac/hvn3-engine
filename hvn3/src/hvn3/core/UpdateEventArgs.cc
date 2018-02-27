#include "hvn3/core/UpdateEventArgs.h"

namespace hvn3 {

	UpdateEventArgs::UpdateEventArgs(double dt) :
		UpdateEventArgs(dt, nullptr) {
	}
	UpdateEventArgs::UpdateEventArgs(double dt, IGameManager* manager) :
		GameStateEventArgs(manager),
		_delta_time(dt) {
	}

	double UpdateEventArgs::Delta() const {

		return _delta_time;

	}

}