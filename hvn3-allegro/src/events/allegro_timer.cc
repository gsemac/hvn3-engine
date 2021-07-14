#include "events/allegro_timer.h"
#include "core/engine.h"

#include <cassert>

#include <allegro5/allegro.h>

namespace hvn3::events {

	// Public members

	AllegroTimer::AllegroTimer(double secondsPerTick, bool enabled) {

		core::Engine::Initialize(core::EngineModules::Core);

		timer = std::shared_ptr<ALLEGRO_TIMER>(al_create_timer(secondsPerTick), FreeTimer);

		assert(static_cast<bool>(timer));

		SetEnabled(enabled);

	}

	void AllegroTimer::SetEnabled(bool enabled) {

		if (enabled)
			al_start_timer(GetUnderlyingData());
		else
			al_stop_timer(GetUnderlyingData());

	}
	bool AllegroTimer::Enabled() const {

		return al_get_timer_started(GetUnderlyingData());

	}
	double AllegroTimer::Period() const {

		return al_get_timer_speed(GetUnderlyingData());

	}
	void AllegroTimer::SetPeriod(double secondsPerTick) {

		al_set_timer_speed(GetUnderlyingData(), secondsPerTick);

	}

	AllegroEventSource AllegroTimer::GetEventSource() const {

		return AllegroEventSource(std::shared_ptr<ALLEGRO_EVENT_SOURCE>(timer, al_get_timer_event_source(GetUnderlyingData())));

	}

	ALLEGRO_TIMER* AllegroTimer::GetUnderlyingData() const {

		return timer.get();

	}

	// Private members

	void AllegroTimer::FreeTimer(ALLEGRO_TIMER* timer) {

		al_destroy_timer(timer);

		core::Engine::Deinitialize(core::EngineModules::Core);

	}

}