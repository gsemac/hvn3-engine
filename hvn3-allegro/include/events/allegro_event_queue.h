#pragma once

#include "events/allegro_event_source.h"
#include "events/event_queue_base.h"

#include <allegro5/allegro.h>

namespace hvn3::events {

	class AllegroEventQueue final :
		public EventQueueBase {

		using underlying_t = ALLEGRO_EVENT_QUEUE;

	public:
		AllegroEventQueue();
		AllegroEventQueue(AllegroEventQueue&& other) noexcept;
		AllegroEventQueue(const AllegroEventQueue& other) = delete;

		~AllegroEventQueue() override;

		bool IsEmpty() const override;
		bool GetNextEvent(Event& ev)override;
		bool PeekNextEvent(Event& ev) const override;
		bool WaitForEvent(Event& ev) override;
		bool WaitForEvent(Event& ev, const TimeSpan& timeout) override;

		void PushEvent(const IUserEvent& ev) override;

		TaggedHandle GetHandle() const override;

		void RegisterEventSource(const AllegroEventSource& eventSource);
		void UnregisterEventSource(const AllegroEventSource& eventSource);

	private:
		underlying_t* eventQueue;
		AllegroEventSource userEventSource;

		void BeforeGetNextEvent(hvn3::events::Event& ev) const;

	};

}