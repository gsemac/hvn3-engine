#pragma once

#include "hvn3/events/IEventArgs.h"

// This class will eventually replace Event as more specific event types are derived from it.

namespace hvn3 {

	class EventArgs :
		public IEventArgs {

	public:
		EventArgs();
		EventArgs(const EventSource& source);
		EventArgs(const EventArgs& other);
		EventArgs(EventArgs&& other);
		~EventArgs();

		const EventSource& Source() const override;
		double Timestamp() const override;
		bool Handled() const override;
		void SetHandled(bool value) override;

	private:
		void _init();

		struct {
			//EventSource __source;
			double timestamp;
			bool handled;
		} _members;

	};

}