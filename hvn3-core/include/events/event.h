#pragma once

#include "events/ievent.h"

#include "allegro5/allegro.h"

#include <utility>

namespace hvn3::events {

	class IUserEvent;

	class Event :
		IEvent {

		using underlying_t = ALLEGRO_EVENT;

	public:
		Event();
		Event(const underlying_t& event);
		~Event();

		EventType Type() const override;
		double Timestamp() const override;

		IUserEvent* GetUserEvent() const;
		underlying_t* GetUnderlyingData();
		const underlying_t* GetUnderlyingData() const;

	private:
		double timestamp;
		underlying_t underlyingEvent;

	};

}