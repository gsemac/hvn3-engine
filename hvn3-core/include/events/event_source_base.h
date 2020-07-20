#pragma once

#include "events/ievent_source.h"

#include <memory>

namespace hvn3::events {

	class EventSourceBase :
		public IEventSource {

		typedef ALLEGRO_EVENT_SOURCE underlying_t;

	public:
		EventSourceBase(underlying_t* eventSource, bool takeOwnership);

		underlying_t* GetUnderlyingData() const override;

		explicit operator bool() const;

	private:
		std::shared_ptr<underlying_t> eventSource;

		static void FreeEventSource(underlying_t* eventSource);

	};

}