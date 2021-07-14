#include "core/allegro_app.h"

#include "events/allegro_event_queue.h"
#include "io/allegro_mouse_manager.h"
#include "events/ievent_manager.h"
#include "graphics/graphics_manager.h"

#include <allegro5/allegro.h>

const double DEFAULT_FPS = 60.0;

namespace hvn3 {

	// Public members

	AllegroApp::AllegroApp() :
		frameTimer(1.0 / DEFAULT_FPS) {
	}

	// Protected members

	void AllegroApp::ConfigureServices(ServiceProvider& services) {

		//services.TryRegisterService<graphics::IGraphicsManager, graphics::GraphicsManager>();

		services.TryRegisterService<hvn3::events::IEventQueue, events::AllegroEventQueue>();

		// Register events service.

		if (!services.IsServiceRegistered<hvn3::events::IEventManager>()) {

			/*if (!services.IsServiceRegistered<hvn3::io::IMouseManager>()) {

				services.RegisterService<hvn3::io::IMouseManager, io::AllegroMouseManager>();

				static_cast<io::AllegroMouseManager&>(services.GetService<hvn3::io::IMouseManager>()).GetEventSource();

			}*/

		}

		if (services.IsServiceRegistered<hvn3::events::IEventManager>())
			al_register_event_source(services.GetService<hvn3::events::IEventManager>().GetEventQueue().GetHandle(), frameTimer.GetEventSource().GetHandle());

		// Ensure required services are instantiated.

		//services.GetService<graphics::IGraphicsManager>();

		// Register remaining services.

		AppBase::ConfigureServices(services);

	}

}