#include "core/game_app.h"

#include "events/ievent_manager.h"

namespace hvn3::core {

	const double FRAMES_PER_SECOND = 60.0;

	// Public members

	GameApp::GameApp() :
		frameTimer(1.0 / FRAMES_PER_SECOND) {

	}

	// Protected members

	void GameApp::ConfigureServices(services::DIServiceContainer& services) {

		AppBase::ConfigureServices(services);

		// Register timer event source.

		if (services.IsServiceRegistered<events::IEventManager>()) {

			services.GetService<events::IEventManager>().GetEventQueue().RegisterEventSource(frameTimer.GetEventSource());

		}

	}

}