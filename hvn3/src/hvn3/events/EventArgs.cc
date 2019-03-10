#include "hvn3/core/Engine.h"
#include "hvn3/events/EventArgs.h"
#include "hvn3/exceptions/Exception.h"

#include <allegro5/allegro.h>

namespace hvn3 {

	// Public methods

	EventArgs::EventArgs() {

		_init();

		_members.timestamp = al_get_time();
		_members.handled = false;

	}
	EventArgs::EventArgs(const EventSource& source) :
		EventArgs() {

		//__source = source;


	}
	EventArgs::EventArgs(const EventArgs& other) {

		_init();

		_members = other._members;

	}
	EventArgs::EventArgs(EventArgs&& other) {

		_init();

		_members = std::move(other._members);

	}
	EventArgs::~EventArgs() {

		hvn3::System::Engine::ReleaseComponent(hvn3::System::EngineComponent::Core);

	}
	const EventSource& EventArgs::Source() const {

		throw System::NotImplementedException();

		//return __source;

	}
	double EventArgs::Timestamp() const {

		return _members.timestamp;

	}
	bool EventArgs::Handled() const {

		return _members.handled;

	}
	void EventArgs::SetHandled(bool value) {

		_members.handled = value;

	}

	// Private methods

	void EventArgs::_init() {

		// Required for al_get_time()
		hvn3::System::Engine::RequireComponent(hvn3::System::EngineComponent::Core);

	}

}