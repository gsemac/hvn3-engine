#pragma once
#include "events/EventSource.h"

// This class will eventually replace Event as more specific event types are derived from it.

namespace hvn3 {
	namespace System {

		class EventArgs {

		public:
			EventArgs();
			EventArgs(const EventSource& source);

			const EventSource& Source() const;
			double Timestamp() const;
			bool Handled() const;
			void SetHandled(bool value);

		private:
			//EventSource __source;
			double _timestamp;
			bool _handled;

		};

	}
}