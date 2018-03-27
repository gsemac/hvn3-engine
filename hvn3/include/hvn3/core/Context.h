#pragma once

namespace hvn3 {

	namespace System {
		class IContextProvider;
		struct Properties;
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
		Context();
		Context(System::IContextProvider* context_provider);

		IObjectManager& GetObjects();
		IRoom& GetRoom();
		RoomManager& GetRooms();
		ICollisionManager<IObject>& GetCollisions();
		Physics::IPhysicsManager& GetPhysics();
		System::IContextProvider& GetContextProvider();
		System::Properties& GetProperties();

		template <typename manager_type>
		manager_type& Get() {
			return static_cast<manager_type&>(_context_provider->GetManagerById(manager_type::Id()));
		}

	private:
		System::IContextProvider* _context_provider;

	};

}