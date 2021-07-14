#pragma once

#include "core/tagged_handle.h"
#include "events/iuser_event.h"

#include <allegro5/allegro.h>

#include <memory>

namespace hvn3::events {

	class AllegroEventSource final {

		using underlying_t = ALLEGRO_EVENT_SOURCE;

	public:
		AllegroEventSource();
		AllegroEventSource(underlying_t* eventSource, bool free);
		AllegroEventSource(const std::shared_ptr<underlying_t>& eventSource);

		void PushEvent(const IUserEvent& ev) const;

		TaggedHandle GetHandle() const;

		explicit operator bool() const;

	private:
		std::shared_ptr<underlying_t> eventSource;
		bool canPushEvents;

		static void FreeEventSource(underlying_t* userEventSource);
		void PushEvent(IUserEvent* event) const;
		static void FreeEvent(ALLEGRO_USER_EVENT* event);

	};

}