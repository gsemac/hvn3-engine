#pragma once

#include "events/ievent_source.h"

namespace hvn3::events {

	class ReadOnlyEventSource :
		public IEventSource {

	public:
		ReadOnlyEventSource();
		ReadOnlyEventSource(underlying_t* eventSource);

		underlying_t* GetUnderlyingData() const override;

		explicit operator bool() const;

	private:
		underlying_t* eventSource;

	};

}