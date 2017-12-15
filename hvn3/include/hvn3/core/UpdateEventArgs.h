#pragma once
#include "hvn3/core/GameStateEventArgs.h"

namespace hvn3 {

	class UpdateEventArgs : public System::GameStateEventArgs {

	public:
		UpdateEventArgs(float dt);
		UpdateEventArgs(float dt, IGameManager* manager);

		// Returns the number of seconds elapsed since the last update.
		float Delta() const;

	private:
		float _delta_time;

	};

}