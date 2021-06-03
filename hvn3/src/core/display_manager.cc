#include "io/display_manager.h"

namespace hvn3::io {

	// Public members

	DisplayManager::DisplayManager(events::IEventManager& eventManager) :
		eventManager(eventManager) {
	}

	IDisplay& DisplayManager::GetDisplay() {

		InitializeDisplay();

		return *display;

	}
	const IDisplay& DisplayManager::GetDisplay() const {

		InitializeDisplay();

		return *display;

	}
	void DisplayManager::SetDisplayOptions(const DisplayOptions& displayOptions) {

		this->displayOptions = displayOptions;

	}

	// Private members

	void DisplayManager::InitializeDisplay() const {

		if (!display) {

			display = std::make_shared<AllegroDisplay>(displayOptions);

			al_register_event_source(eventManager.GetEventQueue().GetHandle(), al_get_display_event_source(display->GetHandle()));

		}

	}

}