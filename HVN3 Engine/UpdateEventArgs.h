#pragma once
#include "EventArgs.h"

namespace hvn3 {

	class UpdateEventArgs : public System::EventArgs {

	public:
		UpdateEventArgs(float dt);

		// Returns the number of seconds elapsed since the last update.
		float Delta() const;

	private:
		float __delta_time;

	};

}