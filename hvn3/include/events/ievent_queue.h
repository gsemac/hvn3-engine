#pragma once

#include "core/tagged_handle.h"
#include "core/time_span.h"
#include "events/event.h"
#include "events/iuser_event.h"

namespace hvn3::events {

	class IEventQueue {

	public:
		virtual ~IEventQueue() = default;

		virtual bool IsEmpty() const = 0;
		virtual bool GetNextEvent(Event& ev) = 0;
		virtual bool PeekNextEvent(Event& ev) const = 0;
		virtual bool WaitForEvent(Event& ev) = 0;
		virtual bool WaitForEvent(Event& ev, const TimeSpan& timeout) = 0;

		virtual void PushEvent(const IUserEvent& event) = 0;

		virtual TaggedHandle GetHandle() const = 0;

		virtual explicit operator bool() const = 0;

	};

}