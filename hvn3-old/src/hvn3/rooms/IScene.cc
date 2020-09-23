#include "hvn3/rooms/IScene.h"

namespace hvn3::scenes {

	// Public members

	IScene::EnterEventArgs::EnterEventArgs(services::DIServiceContainer& services) :
		services(&services) {
	}
	services::DIServiceContainer& IScene::EnterEventArgs::Services() {

		return *services;

	}

	IScene::ExitEventArgs::ExitEventArgs(services::DIServiceContainer& services) :
		services(&services) {
	}
	services::DIServiceContainer& IScene::ExitEventArgs::Services() {

		return *services;

	}

}