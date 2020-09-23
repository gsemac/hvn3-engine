#pragma once

#include "hvn3/events/EventSource.h"

#include <cstdint>

struct ALLEGRO_TIMER;

namespace hvn3 {

	class Timer {

	public:
		Timer(double seconds_per_tick);
		~Timer();

		void Start();
		void Resume();
		void Stop();

		bool HasStarted() const;

		int64_t Count() const;
		void SetCount(int64_t count);
		void AddCount(int64_t diff);

		double SecondsPerTick() const;
		void SetSecondsPerTick(double seconds_per_tick);

		EventSource EventSource();

	private:
		ALLEGRO_TIMER* _timer;

	};

}