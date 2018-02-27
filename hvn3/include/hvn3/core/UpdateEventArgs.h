#pragma once
#include "hvn3/core/GameStateEventArgs.h"

namespace hvn3 {

	class UpdateEventArgs : public System::GameStateEventArgs {

	public:
		UpdateEventArgs(double dt);
		UpdateEventArgs(double dt, IGameManager* manager);

		// Returns the number of seconds elapsed since the last update.
		double Delta() const;

	private:
		double _delta_time;

	};

}