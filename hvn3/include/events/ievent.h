#pragma once

#include "events/event_type.h"

namespace hvn3::events {

	class IEvent {

	public:
		virtual ~IEvent() = default;

		virtual EventType Type() const = 0;
		virtual double Timestamp() const = 0;

	};

}