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
		return *GetRooms().CurrentRoom();
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
	System::IContextProvider& Context::GetContextProvider() {
		return *_context_provider;
	}
	System::Properties& Context::GetProperties() {
		return GetContextProvider().GetGameManager().GetProperties();
	}


}