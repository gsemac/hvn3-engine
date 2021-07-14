#pragma once

#include "allegro_event_source.h"

#include <memory>

struct ALLEGRO_TIMER;

namespace hvn3::events {

	class AllegroTimer {

	public:
		AllegroTimer(double secondsPerTick, bool enabled = true);

		void SetEnabled(bool enabled);
		bool Enabled() const;
		double Period() const;
		void SetPeriod(double secondsPerTick);

		AllegroEventSource GetEventSource() const;
		ALLEGRO_TIMER* GetUnderlyingData() const;

	private:
		std::shared_ptr<ALLEGRO_TIMER> timer;

		static void FreeTimer(ALLEGRO_TIMER* timer);

	};

}