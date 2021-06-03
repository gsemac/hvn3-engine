#pragma once

#include "events/ievent_queue.h"

namespace hvn3::events {

	class EventQueueBase :
		public IEventQueue {

	public:
		explicit operator bool() const override;

	protected:
		EventQueueBase() {}

	};

}