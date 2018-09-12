#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/Context.h"
#include "hvn3/core/IContextProvider.h"
#include "hvn3/core/IGameManager.h"
#include "hvn3/rooms/IRoom.h"
#include "hvn3/rooms/RoomManager.h"

namespace hvn3 {

	Context::Context() {
		_context_provider = nullptr;
	}
	Context::Context(System::IContextProvider* context_provider) {
		_context_provider = context_provider;
	}

	IObjectManager& Context::Objects() {
		return Room().GetObjects();
	}
	IRoom& Context::Room() {
		return *Rooms().Room();
	}
	RoomManager& Context::Rooms() {
		return ContextProvider().GetRoomManager();
	}
	ICollisionManager& Context::Collisions() {
		return Room().GetCollisions();
	}
	Physics::IPhysicsManager& Context::Physics() {
		return Room().GetPhysics();
	}
	IViewManager& Context::Views() {
		return Room().GetViews();
	}
	System::IContextProvider& Context::ContextProvider() {

		if (_context_provider == nullptr)
			throw System::NullReferenceException("Context has not yet been initialized.");

		return *_context_provider;

	}
	GameProperties& Context::Properties() {
		return ContextProvider().GetGameManager().GetProperties();
	}
	Display& Context::Display() {
		return ContextProvider().GetGameManager().GetDisplay();
	}
	IGameManager& Context::GameManager() {
		return ContextProvider().GetGameManager();
	}
	Context::operator bool() const {
		return _context_provider != nullptr;
	}

}