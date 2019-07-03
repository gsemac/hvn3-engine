#include "hvn3/events/EventListenerRegistry.h"
#include "hvn3/events/IUserEvent.h"

namespace hvn3 {

	void EventListenerRegistry::Dispatch(IUserEvent& ev) {
		ev.Dispatch(this);
	}
	EventListenerRegistry::size_type EventListenerRegistry::Count() const {

		size_type count = 0;

		for (auto i = _registry.begin(); i != _registry.end(); ++i)
			count += i->second->Count();

		return count;

	}

}