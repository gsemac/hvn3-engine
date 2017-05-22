#pragma once

struct ALLEGRO_EVENT_SOURCE;

namespace hvn3 {

	class EventQueue;

	class EventSource {
		friend class EventQueue;

	public:
		EventSource(ALLEGRO_EVENT_SOURCE* source);

	private:
		ALLEGRO_EVENT_SOURCE* __ev_source;

	};

}