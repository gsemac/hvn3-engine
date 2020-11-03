#include "core/display_manager.h"

namespace hvn3::core {

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

			display = std::make_shared<Window>(displayOptions);

			eventManager.GetEventQueue().RegisterEventSource(display->GetEventSource());

		}

	}

}