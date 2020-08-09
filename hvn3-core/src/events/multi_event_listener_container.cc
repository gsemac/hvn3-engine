#include "events/multi_event_listener_container.h"
#include "events/iuser_event.h"

namespace hvn3::events {

	void MultiEventListenerContainer::Dispatch(IUserEvent& ev) {

		ev.Dispatch(*this);

	}
	MultiEventListenerContainer::size_type MultiEventListenerContainer::Count() const {

		size_type count = 0;

		for (auto i = _registry.begin(); i != _registry.end(); ++i)
			count += i->second->Count();

		return count;

	}

}