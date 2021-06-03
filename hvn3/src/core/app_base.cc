#include "core/app_base.h"

#include "io/display_manager.h"
#include "events/event_bus.h"
#include "events/event_manager.h"

#include <cstdint>
#include <cstdlib>

namespace hvn3 {

	// Public members

	void AppBase::Show(const io::DisplayOptions& displayOptions) {

		AddWindow(displayOptions);

	}
	int AppBase::Run() {

		for (auto& services : services)
			services.GetService<events::IEventManager>().AddEventFilter(std::make_shared<EventFilter>(services));

		return DoEventLoop();

	}
	int AppBase::Run(const io::DisplayOptions& displayOptions) {

		AddWindow(displayOptions);

		return Run();

	}

	// Protected members

	AppBase::EventFilter::EventFilter(core::DIServiceContainer& services) :
		displayId(-1) {

		if (services.IsServiceRegistered<io::IDisplayManager>())
			displayId = services.GetService<io::IDisplayManager>().GetDisplay().Id();

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

	void AppBase::ConfigureServices(core::DIServiceContainer& services) {

		services.TryRegisterService<io::IDisplayManager, io::DisplayManager>();

		services.TryRegisterService<events::IEventBus, events::MultiEventBus>();
		services.TryRegisterService<events::IEventManager, events::EventManager>();

	}

	// Private members

	void AppBase::AddWindow(const io::DisplayOptions& displayOptions) {

		core::DIServiceContainer services;

		ConfigureServices(services);

		// Ensure that there is an event manager registered.

		if (!services.IsServiceRegistered<events::IEventManager>())
			services.RegisterService<events::IEventManager, events::EventManager>();

		// Apply the user's display options.

		if (services.IsServiceRegistered<io::IDisplayManager>())
			services.GetService<io::IDisplayManager>().SetDisplayOptions(displayOptions);

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