#pragma once

#include "events/ievent.h"

namespace hvn3::events {

	template<EventType EVENT_TYPE = EventType::Unspecified>
	class EventBase :
		public IEvent {

	public:
		EventType Type() const override {

			return EVENT_TYPE;

		}
		double Timestamp() const override {

			return timestamp;

		}

	protected:
		EventBase() :
			timestamp(0.0) {
		}

	private:
		double timestamp;

	};

}