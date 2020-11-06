#pragma once

#include "events/ievent_source.h"

namespace hvn3::io {

	class IMouseManager {

	public:
		virtual ~IMouseManager() = default;

		virtual const events::IEventSource& GetEventSource() const = 0;

	};

}