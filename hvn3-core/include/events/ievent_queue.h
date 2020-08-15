#pragma once

#include "core/time_span.h"
#include "events/event.h"
#include "events/ievent_source.h"

namespace hvn3::events {

	class IEventQueue {

	public:
		virtual ~IEventQueue() = default;

		virtual void RegisterEventSource(const IEventSource& eventSource) = 0;
		virtual void UnregisterEventSource(const IEventSource& eventSource) = 0;

		virtual bool IsEmpty() const = 0;
		virtual bool GetNextEvent(Event& ev) = 0;
		virtual bool PeekNextEvent(Event& ev) const = 0;
		virtual void WaitForEvent(Event& ev) = 0;
		virtual bool WaitForEvent(Event& ev, const core::TimeSpan& timeout) = 0;

	};

}