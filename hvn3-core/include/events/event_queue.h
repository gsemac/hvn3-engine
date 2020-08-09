#pragma once

#include "core/time_span.h"
#include "events/event.h"
#include "events/ievent_source.h"

struct ALLEGRO_EVENT_QUEUE;

namespace hvn3::events {

	class EventQueue {

		using underlying_t = ALLEGRO_EVENT_QUEUE;

	public:
		EventQueue();
		EventQueue(EventQueue&& other) noexcept;
		EventQueue(const EventQueue& other) = delete;

		~EventQueue();

		void RegisterEventSource(const IEventSource& eventSource);
		void UnregisterEventSource(const IEventSource& eventSource);

		bool IsEmpty() const;
		bool GetNextEvent(Event& ev);
		bool PeekNextEvent(Event& ev) const;
		void WaitForEvent(Event& ev);
		bool WaitForEvent(Event& ev, const core::TimeSpan& timeout);

		underlying_t* GetUnderlyingData();

		explicit operator bool() const;

	private:
		underlying_t* eventQueue;

	};

}