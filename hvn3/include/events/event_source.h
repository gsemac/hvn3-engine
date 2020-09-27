#pragma once

#include "events/ievent_source.h"

#include <memory>

namespace hvn3::events {

	class EventSource :
		public IEventSource {

	public:
		EventSource();
		EventSource(underlying_t* eventSource);
		EventSource(std::shared_ptr<underlying_t> eventSource);

		underlying_t* GetUnderlyingData() const override;

		explicit operator bool() const;

	private:
		std::shared_ptr<underlying_t> eventSource;

	};

}