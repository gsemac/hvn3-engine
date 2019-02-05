#pragma once

#include "hvn3/events/IEventArgs.h"

// This class will eventually replace Event as more specific event types are derived from it.

namespace hvn3 {

	class EventArgs :
		public IEventArgs {

	public:
		EventArgs();
		EventArgs(const EventSource& source);

		const EventSource& Source() const override;
		double Timestamp() const override;
		bool Handled() const override;
		void SetHandled(bool value) override;

	private:
		//EventSource __source;
		double _timestamp;
		bool _handled;

	};

}