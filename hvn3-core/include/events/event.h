#pragma once

#include "events/ievent.h"

#include "allegro5/allegro.h"

#include <utility>

namespace hvn3::events {

	class Event :
		IEvent {

		using underlying_t = ALLEGRO_EVENT;

	public:
		Event() :
			timestamp(0.0),
			underlyingEvent() {
		}
		Event(const underlying_t& event) :
			Event() {

			underlyingEvent = event;

		}

		EventType Type() override {

			return static_cast<EventType>(underlyingEvent.type);

		}
		double Timestamp() override {

			return timestamp;

		}

		underlying_t* GetUnderlyingData() {

			return &underlyingEvent;

		}

	private:
		double timestamp;
		underlying_t underlyingEvent;

	};

}