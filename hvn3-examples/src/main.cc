#include <iostream>

#include "core/application.h"
#include "core/window.h"
#include "events/display_events.h"
#include "events/event_listener_base.h"
#include "events/event_manager.h"
#include "events/event_queue.h"
#include "events/multi_event_bus.h"
#include "events/user_event.h"
#include "events/user_event_source.h"
#include "services/di_service_container.h"

using namespace hvn3;

class MyApplication :
	public core::Application {

protected:
	void ConfigureServices(services::DIServiceContainer& services) {

		Application::ConfigureServices(services);

		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::DisplayCloseEvent>(
			[](events::DisplayCloseEvent& e) {

				std::cout << "Display closed\n";

			}
		);

	}

};

int main() {

	core::Window window(640, 480, "hello world", core::WindowOptions::Resizable);

	MyApplication app;

	app.Run(window);

	getchar();

	return 0;

}