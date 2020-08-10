#include "events/multi_event_dispatcher.h"
#include "events/iuser_event.h"

namespace hvn3::events {

	void MultiEventDispatcher::Dispatch(IUserEvent& ev) {

		ev.Dispatch(*this);

	}

	bool MultiEventDispatcher::Unsubscribe(void* eventListener) {

		bool unsubbed = false;

		for (auto i = _registry.begin(); i != _registry.end(); ++i)
			unsubbed = i->second->Unsubscribe(eventListener) || unsubbed;

		return unsubbed;

	}

	MultiEventDispatcher::size_type MultiEventDispatcher::Count() const {

		size_type count = 0;

		for (auto i = _registry.begin(); i != _registry.end(); ++i)
			count += i->second->Count();

		return count;

	}

}