#include "core/app_base.h"
#include "core/display_manager.h"

#include "events/event_manager.h"

namespace hvn3::core {

	// Public members

	void AppBase::Run(const DisplayOptions& displayOptions) {

		InitializeApp(displayOptions);

		Run();

	}

	// Protected members

	void AppBase::ConfigureServices(services::DIServiceContainer& services) {

		services.RegisterService<events::IEventManager, events::EventManager>();
		services.RegisterService<core::IDisplayManager, core::DisplayManager>();

	}

	// Private members

	void AppBase::InitializeApp(const DisplayOptions& displayOptions) {

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
	void AppBase::Run() {

		bool handleEvents = true;

		while (handleEvents) {

			auto& eventManager = services[0].GetService<events::IEventManager>();

			eventManager.DoEvents(true);

		}

	}

}