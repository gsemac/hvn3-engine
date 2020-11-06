#include "core/app_base.h"
#include "core/display_manager.h"
#include "events/event_manager.h"

#include <cstdint>
#include <cstdlib>

namespace hvn3::core {

	// Public members

	void AppBase::Show(const DisplayOptions& displayOptions) {

		AddWindow(displayOptions);

	}
	int AppBase::Run() {

		for (auto& services : services)
			services.GetService<events::IEventManager>().AddEventFilter(std::make_shared<EventFilter>(services));

		return DoEventLoop();

	}
	int AppBase::Run(const DisplayOptions& displayOptions) {

		AddWindow(displayOptions);

		return Run();

	}

	// Protected members

	AppBase::EventFilter::EventFilter(services::DIServiceContainer& services) :
		displayId(-1) {

		if (services.IsServiceRegistered<core::IDisplayManager>())
			displayId = services.GetService<core::IDisplayManager>().GetDisplay().Id();

	}
	bool AppBase::EventFilter::PreFilterEvent(events::Event& ev) const {

		int eventDisplayId = GetDisplayId(ev);

		if (displayId != -1 && eventDisplayId != -1 && displayId != eventDisplayId)
			return true;

		return false;

	}
	bool AppBase::EventFilter::IsMouseEvent(events::Event& ev) const {

		switch (ev.Type()) {

		case events::EventType::MouseAxes:
			return true;

		}

		return false;

	}
	int AppBase::EventFilter::GetDisplayId(events::Event& ev) const {

		if (IsMouseEvent(ev))
			return static_cast<int32_t>(reinterpret_cast<uintptr_t>(ev.GetUnderlyingData()->mouse.display));

		return -1;

	}

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