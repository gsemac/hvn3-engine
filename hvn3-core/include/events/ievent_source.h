#pragma once

#include <allegro5/allegro.h>

namespace hvn3::events {

	class IEventSource {

	public:
		virtual ALLEGRO_EVENT_SOURCE* GetUnderlyingData() const = 0;

	};

}