#include <iostream>

//#include "core/allegro_app.h"
//#include "core/window.h"
//#include "events/display_events.h"
//#include "events/event_listener_base.h"
//#include "events/event_manager.h"
//#include "events/event_queue.h"
//#include "events/multi_event_bus.h"
//#include "events/user_event.h"
//#include "events/user_event_source.h"
//#include "services/di_service_container.h"
//#include "events/timer_events.h"
//#include "events/draw_events.h"
//#include "core/window_options.h"
//#include "core/random.h"
//#include "events/mouse_events.h"
//#include "core/idisplay_manager.h"

#include "io/allegro_display.h"
#include "core/allegro_app.h"

using namespace hvn3;

class MyApp :
	public AllegroApp {

	//protected:
	//	void ConfigureServices(DIServiceContainer& services) {
	//
	//		GameApp::ConfigureServices(services);
	//
	//		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::DrawEvent>(
	//			[](events::DrawEvent& e) {
	//
	//				//std::cout << "draw\n";
	//
	//				math::RectangleI rect = e.Graphics().GetClip();
	//
	//				e.Graphics().DrawRectangle(rect.x, rect.y, rect.width, rect.height, graphics::Color::DarkBlue, 3.0f);
	//
	//			}
	//		);
	//
	//		services.GetService<events::IEventManager>().GetEventBus().Subscribe<events::MouseMoveEvent>(
	//			[=](events::MouseMoveEvent& e) {
	//
	//				std::cout << services.GetService<core::IDisplayManager>().GetDisplay().Id() << ": mouse at " << e.Position() << '\n';
	//
	//			}
	//		);
	//
	//	}

};

int main() {

	io::DisplayOptions displayOptions;

	displayOptions.SetSize(640, 480);
	displayOptions.SetTitle("hello world");
	displayOptions.SetWindowOptions(io::WindowOptions::Resizable | io::WindowOptions::UseOpenGL);

	AllegroApp app;

	app.Show(displayOptions);

	displayOptions.SetPosition(200, 200);

	app.Show(displayOptions);

	return app.Run();

	return 0;

}