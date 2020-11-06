#include "core/game_app.h"

#include "events/ievent_manager.h"
#include "graphics/graphics_manager.h"
#include "io/mouse_manager.h"

namespace hvn3::core {

	const double FRAMES_PER_SECOND = 60.0;

	// Public members

	GameApp::GameApp() :
		frameTimer(1.0 / FRAMES_PER_SECOND) {
	}

	// Protected members

	void GameApp::ConfigureServices(services::DIServiceContainer& services) {

		AppBase::ConfigureServices(services);

		// Register the graphics service.

		if (!services.IsServiceRegistered<graphics::IGraphicsManager>())
			services.RegisterService<graphics::IGraphicsManager, graphics::GraphicsManager>();

		// Register IO services.

		if (!services.IsServiceRegistered<io::IMouseManager>())
			services.RegisterService<io::IMouseManager, io::MouseManager>();

		// Register timer event source.

		if (services.IsServiceRegistered<events::IEventManager>())
			services.GetService<events::IEventManager>().GetEventQueue().RegisterEventSource(frameTimer.GetEventSource());

		// Ensure required services are instantiated.

		services.GetService<graphics::IGraphicsManager>();
		services.GetService<io::IMouseManager>();

	}

}