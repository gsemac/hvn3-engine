#pragma once

#include "events/ievent.h"

#include "allegro5/allegro.h"

#include <utility>

namespace hvn3::events {

	class Event :
		IEvent {

		using underlying_t = ALLEGRO_EVENT;

	public:
		Event();
		Event(const underlying_t& event);
		~Event();

		EventType Type() const override;
		double Timestamp() const override;

		underlying_t* GetUnderlyingData();

	private:
		double timestamp;
		underlying_t underlyingEvent;

	};

}