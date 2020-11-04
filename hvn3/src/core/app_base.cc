#include "core/app_base.h"
#include "core/display_manager.h"
#include "events/event_manager.h"

#include <cstdlib>

namespace hvn3::core {

	// Public members

	void AppBase::Show(const DisplayOptions& displayOptions) {

		AddWindow(displayOptions);

	}
	int AppBase::Run() {

		return DoEventLoop();

	}
	int AppBase::Run(const DisplayOptions& displayOptions) {

		AddWindow(displayOptions);

		return Run();

	}

	// Protected members

	void AppBase::ConfigureServices(services::DIServiceContainer& services) {

		services.RegisterService<events::IEventManager, events::EventManager>();
		services.RegisterService<core::IDisplayManager, core::DisplayManager>();

	}

	// Private members

	void AppBase::AddWindow(const DisplayOptions& displayOptions) {

		services::DIServiceContainer services;

		ConfigureServices(services);

		// Ensure that there is an event manager registered.

		if (!services.IsServiceRegistered<events::IEventManager>())
			services.RegisterService<events::IEventManager, events::EventManager>();

		// Apply the user's display options.

		if (services.IsServiceRegistered<core::IDisplayManager>())
			services.GetService<core::IDisplayManager>().SetDisplayOptions(displayOptions);

		this->services.push_back(std::move(services));

	}
	int AppBase::DoEventLoop() {

		bool handleEvents = true;

		while (handleEvents) {

			for (auto& services : services) {

				auto& eventManager = services.GetService<events::IEventManager>();

				eventManager.DoEvents(true);

			}

		}

		return EXIT_SUCCESS;

	}

}