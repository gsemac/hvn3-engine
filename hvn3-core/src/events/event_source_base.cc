#include "events/event_source_base.h"

#include "core/engine.h"

namespace hvn3::events {

	// Public members

	EventSourceBase::EventSourceBase(underlying_t* eventSource, bool takeOwnership) {

		if (eventSource != nullptr) {

			if (takeOwnership) {

				core::Engine::Initialize(core::EngineModules::Core);

				this->eventSource = std::shared_ptr<underlying_t>(eventSource, FreeEventSource);

			}
			else {

				this->eventSource = std::shared_ptr<underlying_t>(std::shared_ptr<underlying_t>{}, eventSource);

			}

		}

	}

	EventSourceBase::underlying_t* EventSourceBase::GetUnderlyingData() const {

		return eventSource.get();

	}

	EventSourceBase::operator bool() const {

		return GetUnderlyingData() != nullptr;

	}

	// Private members

	void EventSourceBase::FreeEventSource(underlying_t* eventSource) {

		if (eventSource != nullptr) {

			al_destroy_user_event_source(eventSource);

			core::Engine::Deinitialize(core::EngineModules::Core);

		}

	}

}