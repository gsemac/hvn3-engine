#include "core/application.h"
#include "events/event_manager.h"

#include <utility>

namespace hvn3 {

	// Public members

	Application::WindowInfo::WindowInfo(const graphics::Window& window) :
		window(window) {
	}

	void Application::Run(const graphics::Window& window) {

		InitializeWindow(window);

		Run();

	}

	// Private members

	void Application::InitializeWindow(const graphics::Window& window) {

		WindowInfo windowInfo(window);

		windowInfo.services.RegisterService<events::IEventManager, events::EventManager>();

		auto& eventManager = windowInfo.services.GetService<events::IEventManager>();

		eventManager.GetEventQueue().RegisterEventSource(window.GetEventSource());

		windows.push_back(std::move(windowInfo));

	}
	void Application::Run() {

		bool handleEvents = true;

		while (handleEvents) {

			auto& eventManager = windows[0].services.GetService<events::IEventManager>();

			eventManager.DoEvents(true);

		}

	}

}