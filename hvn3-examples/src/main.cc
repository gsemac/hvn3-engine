#include <iostream>

#include "core/game_app.h"
#include "core/window.h"
#include "events/display_events.h"
#include "events/event_listener_base.h"
#include "events/event_manager.h"
#include "events/event_queue.h"
#include "events/multi_event_bus.h"
#include "events/user_event.h"
#include "events/user_event_source.h"
#include "services/di_service_container.h"
#include "events/timer_events.h"
#include "events/graphics_events.h"

using namespace hvn3;

class MyApp :
	public core::GameApp {

protected:
	void ConfigureServices(services::DIServiceContainer& services) {

		GameApp::ConfigureServices(services);

		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::DisplayCloseEvent>(
			[](events::DisplayCloseEvent& e) {

				//std::cout << "Display closed\n";

			}
		);

		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::TickEvent>(
			[](events::TickEvent& e) {

				//std::cout << "timer tick\n";

			}
		);

		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::DrawFrameEvent>(
			[](events::DrawFrameEvent& e) {

				std::cout << "frame draw\n";

			}
		);

		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::DrawEvent>(
			[](events::DrawEvent& e) {

				std::cout << "draw\n";

			}
		);

	}

};

int main() {

	core::Window window(640, 480, "hello world", core::WindowOptions::Resizable);

	MyApp app;

	app.Run(window);

	getchar();

	return 0;

}