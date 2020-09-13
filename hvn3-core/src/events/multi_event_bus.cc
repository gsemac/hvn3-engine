#include "events/multi_event_bus.h"
#include "events/iuser_event.h"

namespace hvn3::events {

	void MultiEventBus::Dispatch(IUserEvent& ev) {

		ev.DispatchTo(*this);

	}

	bool MultiEventBus::Unsubscribe(void* eventListener) {

		bool unsubbed = false;

		for (auto i = _registry.begin(); i != _registry.end(); ++i)
			unsubbed = i->second->Unsubscribe(eventListener) || unsubbed;

		return unsubbed;

	}

	MultiEventBus::size_type MultiEventBus::Count() const {

		size_type count = 0;

		for (auto i = _registry.begin(); i != _registry.end(); ++i)
			count += i->second->Count();

		return count;

	}

}