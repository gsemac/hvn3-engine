#pragma once

#include "events/event.h"

namespace hvn3::events {

	class IEventFilter {

	public:
		virtual bool PreFilterEvent(Event& ev) const = 0;

	};

}