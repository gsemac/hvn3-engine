#pragma once

#include "core/type_index.h"
#include "events/ievent.h"

namespace hvn3::events {

	class MultiEventListenerContainer;

	class IUserEvent :
		IEvent {

	public:
		template<typename T>
		using event_index = core::TypeIndex<T, struct event_indexer_family>;
		using event_id_type = int;

		virtual ~IUserEvent() = default;

		virtual void* Data() = 0;

		virtual void Dispatch(const MultiEventListenerContainer& eventListenerContainer) = 0;

	};

}