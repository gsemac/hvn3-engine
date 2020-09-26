#include "core/app_base.h"

#include "events/event_manager.h"

namespace hvn3::core {

	// Public members

	AppBase::WindowInfo::WindowInfo(const Window& window) :
		window(window) {
	}

	void AppBase::Run(const Window& window) {

		InitializeWindow(window);

		Run();

	}

	// Protected members

	void AppBase::ConfigureServices(services::DIServiceContainer& services) {

		services.RegisterService<events::IEventManager, events::EventManager>();

	}

	// Private members

	void AppBase::InitializeWindow(const Window& window) {

		WindowInfo windowInfo(window);

		ConfigureServices(windowInfo.services);

		if (windowInfo.services.IsServiceRegistered<events::IEventManager>()) {

			auto& eventManager = windowInfo.services.GetService<events::IEventManager>();

			eventManager.GetEventQueue().RegisterEventSource(window.GetEventSource());

		}

		windows.push_back(std::move(windowInfo));

	}
	void AppBase::Run() {

		bool handleEvents = true;

		while (handleEvents) {

			auto& eventManager = windows[0].services.GetService<events::IEventManager>();

			eventManager.DoEvents(true);

		}

	}

}