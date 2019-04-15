#pragma once

#include "hvn3/utility/TypeIndexer.h"

namespace hvn3 {

	class IUserEvent {

	public:
		typedef TypeIndexer<struct event_indexer_family> event_indexer;
		typedef typename event_indexer::index_type event_id;

		virtual event_id Id() const = 0;
		virtual void* Data() = 0;

	};

}