#pragma once

#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class UpdateEventArgs : 
		public EventArgs {

	public:
		UpdateEventArgs(const ApplicationContext& context, double deltaTime);

		// Returns the number of seconds elapsed since the last update.
		double Delta() const;

	private:
		double _delta_time;

	};

	class UpdateBeginEventArgs : UpdateEventArgs {
	public:
		using UpdateEventArgs::UpdateEventArgs;
	};

	class UpdateEndEventArgs : UpdateEventArgs {
	public:
		using UpdateEventArgs::UpdateEventArgs;
	};

}