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

	IObjectManager& Context::GetObjects() {
		return GetRoom().GetObjects();
	}
	IRoom& Context::GetRoom() {
		return *GetRooms().Room();
	}
	RoomManager& Context::GetRooms() {
		return GetContextProvider().GetRoomManager();
	}
	ICollisionManager<IObject>& Context::GetCollisions() {
		return GetRoom().GetCollisions();
	}
	Physics::IPhysicsManager& Context::GetPhysics() {
		return GetRoom().GetPhysics();
	}
	IViewManager& Context::GetViews() {
		return GetRoom().GetViews();
	}
	System::IContextProvider& Context::GetContextProvider() {

		if (_context_provider == nullptr)
			throw System::NullReferenceException("Context has not yet been initialized.");

		return *_context_provider;

	}
	System::Properties& Context::GetProperties() {
		return GetContextProvider().GetGameManager().GetProperties();
	}
	Display& Context::GetDisplay() {
		return GetContextProvider().GetGameManager().GetDisplay();
	}
	IGameManager& Context::GetGameManager() {
		return GetContextProvider().GetGameManager();
	}
	Context::operator bool() const {
		return _context_provider != nullptr;
	}

}