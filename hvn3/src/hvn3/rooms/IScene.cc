#include "hvn3/rooms/IScene.h"

namespace hvn3 {

	// IScene::CreateEventArgs

	// Public methods

	IScene::CreateEventArgs::CreateEventArgs(const ApplicationContext& context) :
		_context(context) {}
	ApplicationContext IScene::CreateEventArgs::Context() const {

		return _context;

	}

	// IScene::EnterEventArgs

	// Public methods

	IScene::EnterEventArgs::EnterEventArgs(const ApplicationContext& context) :
		_context(context) {}
	ApplicationContext IScene::EnterEventArgs::Context() const {

		return _context;

	}

	// IScene::ExitEventArgs

	// Public methods

	IScene::ExitEventArgs::ExitEventArgs(const ApplicationContext& context) :
		_context(context) {}
	ApplicationContext IScene::ExitEventArgs::Context() const {

		return _context;

	}

}