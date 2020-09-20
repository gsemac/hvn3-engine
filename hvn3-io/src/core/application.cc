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

		Run();

	}

	// Private members

	void Application::InitializeWindow(const io::Window& window) {

		WindowInfo windowInfo(window);

		windowInfo.services.RegisterService<events::IEventManager, events::EventManager>();

		auto& eventManager = windowInfo.services.GetService<events::IEventManager>();

		eventManager.GetEventQueue().RegisterEventSource(window.GetEventSource());

		windows.push_back(std::move(windowInfo));

	}
	void Application::Run() {

		events::Event ev;

		while (true) {

			auto& eventManager = windows[0].services.GetService<events::IEventManager>();

			if (eventManager.GetEventQueue().WaitForEvent(ev))
				std::cout << (int)ev.Type() << std::endl;

		}

	}

}