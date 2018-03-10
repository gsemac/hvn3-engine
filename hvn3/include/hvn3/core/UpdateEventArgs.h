#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class UpdateEventArgs : public System::EventArgs {

	public:
		UpdateEventArgs(double dt);

		// Returns the number of seconds elapsed since the last update.
		double Delta() const;

	private:
		double _delta_time;

	};

}