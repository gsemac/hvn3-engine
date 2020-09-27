#pragma once

#include "events/event_source.h"

#include <memory>

struct ALLEGRO_TIMER;

namespace hvn3::events {

	class Timer {

	public:
		Timer(double secondsPerTick, bool enabled = true);

		void SetEnabled(bool enabled);
		bool Enabled() const;
		double Period() const;
		void SetPeriod(double secondsPerTick);

		EventSource GetEventSource() const;

		ALLEGRO_TIMER* GetUnderlyingData() const;

	private:
		std::shared_ptr<ALLEGRO_TIMER> timer;

	};

}