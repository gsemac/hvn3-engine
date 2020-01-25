#pragma once

#include "hvn3/events/IEventArgs.h"

namespace hvn3 {

	class EventArgs :
		public IEventArgs {

	public:
		EventArgs();
		EventArgs(const EventArgs& other);
		EventArgs(EventArgs&& other);
		~EventArgs();

		double Timestamp() const override;
		bool Handled() const override;
		void SetHandled(bool value) override;

	private:
		void _init();

		struct {
			double timestamp;
			bool handled;
		} _members;

	};

}