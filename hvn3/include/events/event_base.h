#pragma once

#include "events/ievent.h"

namespace hvn3::events {

	class EventBase :
		public IEvent {

	public:
		EventType Type() const override;
		double Timestamp() const override;

	protected:
		EventBase();
		EventBase(EventType eventType);

	private:
		EventType eventType;
		double timestamp;

	};

}