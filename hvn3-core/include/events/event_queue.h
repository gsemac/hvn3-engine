#pragma once

#include "core/time_span.h"
#include "events/event.h"
#include "events/ievent_queue.h"
#include "events/ievent_source.h"

struct ALLEGRO_EVENT_QUEUE;

namespace hvn3::events {

	class EventQueue :
		public IEventQueue {

		using underlying_t = ALLEGRO_EVENT_QUEUE;

	public:
		EventQueue();
		EventQueue(EventQueue&& other) noexcept;
		EventQueue(const EventQueue& other) = delete;

		~EventQueue();

		void RegisterEventSource(const IEventSource& eventSource) override;
		void UnregisterEventSource(const IEventSource& eventSource) override;

		bool IsEmpty() const override;
		bool GetNextEvent(Event& ev)override;
		bool PeekNextEvent(Event& ev) const override;
		void WaitForEvent(Event& ev) override;
		bool WaitForEvent(Event& ev, const core::TimeSpan& timeout) override;

		underlying_t* GetUnderlyingData();

		explicit operator bool() const;

	private:
		underlying_t* eventQueue;

		void BeforeGetNextEvent(Event& ev) const;

	};

}