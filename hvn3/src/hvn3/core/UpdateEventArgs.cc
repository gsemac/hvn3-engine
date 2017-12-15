#include "hvn3/core/UpdateEventArgs.h"

namespace hvn3 {

	UpdateEventArgs::UpdateEventArgs(float dt) :
		UpdateEventArgs(dt, nullptr) {
	}
	UpdateEventArgs::UpdateEventArgs(float dt, IGameManager* manager) :
		GameStateEventArgs(manager),
		_delta_time(dt) {
	}

	float UpdateEventArgs::Delta() const {

		return _delta_time;

	}

}