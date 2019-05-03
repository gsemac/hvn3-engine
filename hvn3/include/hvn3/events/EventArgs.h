#pragma once

#include "hvn3/events/IEventArgs.h"

namespace hvn3 {

	class EventArgs :
		public IEventArgs {

	public:
		EventArgs();
		EventArgs(const ApplicationContext& context);
		EventArgs(const EventArgs& other);
		EventArgs(EventArgs&& other);
		~EventArgs();

		double Timestamp() const override;
		bool Handled() const override;
		void SetHandled(bool value) override;
		ApplicationContext Context() override;

	private:
		void _init();

		struct {
			ApplicationContext context;
			double timestamp;
			bool handled;
		} _members;

	};

}