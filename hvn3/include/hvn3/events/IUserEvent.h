#pragma once

#include "hvn3/utility/TypeIndexer.h"

namespace hvn3 {

	class EventListenerRegistry;

	class IUserEvent {

	public:
		typedef TypeIndexer<struct event_indexer_family> event_indexer;
		typedef event_indexer::index_type event_id;

		virtual ~IUserEvent() {}

		virtual event_id Id() const = 0;
		virtual void* Data() = 0;

		virtual void Dispatch(EventListenerRegistry* registry) = 0;

	};

}