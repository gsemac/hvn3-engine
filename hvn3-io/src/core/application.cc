#include "core/application.h"
#include "events/event_manager.h"

#include <utility>

namespace hvn3 {

	// Public members

	Application::WindowInfo::WindowInfo(const io::Window& window) :
		window(window) {
	}

	void Application::Run(const io::Window& window) {

		InitializeWindow(window);

	}

	// Private members

	void Application::InitializeWindow(const io::Window& window) {

		//WindowInfo windowInfo(window);

		//windowInfo.services.RegisterService<events::IEventManager, events::EventManager>();

		//auto& eventManager = windowInfo.services.GetService<events::IEventManager>();

		//windows.push_back(std::move(windowInfo));

	}

}