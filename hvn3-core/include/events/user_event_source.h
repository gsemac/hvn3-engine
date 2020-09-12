#pragma once

#include "events/ievent_source.h"
#include "events/user_event.h"

#include <memory>

struct ALLEGRO_USER_EVENT;

namespace hvn3::events {

	class UserEventSource :
		public IEventSource {

	public:
		UserEventSource();

		void PushEvent(const IUserEvent& event) const;

		underlying_t* GetUnderlyingData() const override;

		explicit operator bool() const;

	private:
		class EventSourceWrapper {

		public:
			underlying_t eventSource;

			EventSourceWrapper();
			~EventSourceWrapper();

		};

		std::shared_ptr<EventSourceWrapper> eventSource;

		void PushEvent(IUserEvent* event) const;

		static void FreeEvent(ALLEGRO_USER_EVENT* event);

	};

}