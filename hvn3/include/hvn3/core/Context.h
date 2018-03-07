#pragma once

namespace hvn3 {

	namespace System {
		class IContextProvider;
	}

	template<typename T>
	class ICollisionManager;
	class IObject;
	class IObjectManager;
	class IRoom;
	class RoomManager;

	namespace Physics {
		class IPhysicsManager;
	}

	class Context {

	public:
		Context(System::IContextProvider* context_provider);

		IObjectManager& GetObjects();
		IRoom& GetRoom();
		RoomManager& GetRooms();
		ICollisionManager<IObject>& GetCollisions();
		Physics::IPhysicsManager& GetPhysics();

	private:
		System::IContextProvider* _context_provider;

	};

}