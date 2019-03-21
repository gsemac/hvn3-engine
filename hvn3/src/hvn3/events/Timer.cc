#include "hvn3/events/Timer.h"
#include <allegro5/allegro.h>

namespace hvn3 {

	Timer::Timer(double seconds_per_tick) {

		_timer = al_create_timer(seconds_per_tick);

	}
	Timer::~Timer() {

		if (_timer)
			al_destroy_timer(_timer);
		_timer = nullptr;

	}
	void Timer::Start() {

		al_start_timer(_timer);

	}
	void Timer::Resume() {

		al_resume_timer(_timer);

	}
	void Timer::Stop() {

		al_stop_timer(_timer);

	}
	bool Timer::HasStarted() const {

		return al_get_timer_started(_timer);

	}
	int64_t Timer::Count() const {

		return al_get_timer_count(_timer);

	}
	void Timer::SetCount(int64_t count) {

		al_set_timer_count(_timer, count);

	}
	void Timer::AddCount(int64_t diff) {

		al_add_timer_count(_timer, diff);

	}
	double Timer::SecondsPerTick() const {

		return al_get_timer_speed(_timer);

	}
	void Timer::SetSecondsPerTick(double seconds_per_tick) {

		return al_set_timer_speed(_timer, seconds_per_tick);

	}
	EventSource Timer::EventSource() {

		return class EventSource(al_get_timer_event_source(_timer));

	}

}