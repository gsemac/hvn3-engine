#include "hvn3/events/Timer.h"
#include <allegro5/allegro.h>

namespace hvn3 {

	Timer::Timer(double seconds_per_tick) {

		__timer = al_create_timer(seconds_per_tick);

	}
	Timer::~Timer() {

		if (__timer)
			al_destroy_timer(__timer);
		__timer = nullptr;

	}
	void Timer::Start() {

		al_start_timer(__timer);

	}
	void Timer::Resume() {

		al_resume_timer(__timer);

	}
	void Timer::Stop() {

		al_stop_timer(__timer);

	}
	bool Timer::HasStarted() const {

		return al_get_timer_started(__timer);

	}
	int64_t Timer::Count() const {

		return al_get_timer_count(__timer);

	}
	void Timer::SetCount(int64_t count) {

		al_set_timer_count(__timer, count);

	}
	void Timer::AddCount(int64_t diff) {

		al_add_timer_count(__timer, diff);

	}
	double Timer::SecondsPerTick() const {

		return al_get_timer_speed(__timer);

	}
	void Timer::SetSecondsPerTick(double seconds_per_tick) {

		return al_set_timer_speed(__timer, seconds_per_tick);

	}
	EventSource Timer::EventSource() {

		return class EventSource(al_get_timer_event_source(__timer));

	}

}