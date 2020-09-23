#pragma once

#include <allegro5/allegro.h>

namespace hvn3::events {

	class IEventSource {

	protected:
		using underlying_t = ALLEGRO_EVENT_SOURCE;

	public:
		virtual ~IEventSource() = default;

		virtual underlying_t* GetUnderlyingData() const = 0;

	};

}