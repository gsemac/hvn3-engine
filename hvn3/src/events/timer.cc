#include "events/timer.h"

#include <allegro5/allegro.h>

namespace hvn3::events {

	Timer::Timer(double secondsPerTick, bool enabled) {

		timer = std::shared_ptr<ALLEGRO_TIMER>(al_create_timer(secondsPerTick), al_destroy_timer);

	}

	void Timer::SetEnabled(bool enabled) {

		if (enabled)
			al_start_timer(GetUnderlyingData());
		else
			al_stop_timer(GetUnderlyingData());

	}
	bool Timer::Enabled() const {

		return al_get_timer_started(GetUnderlyingData());

	}
	double Timer::Period() const {

		return al_get_timer_speed(GetUnderlyingData());

	}
	void Timer::SetPeriod(double secondsPerTick) {

		al_set_timer_speed(GetUnderlyingData(), secondsPerTick);

	}

	EventSource Timer::GetEventSource() const {

		return EventSource(std::shared_ptr<ALLEGRO_EVENT_SOURCE>(timer, al_get_timer_event_source(GetUnderlyingData())));

	}

	ALLEGRO_TIMER* Timer::GetUnderlyingData() const {

		return timer.get();

	}

}