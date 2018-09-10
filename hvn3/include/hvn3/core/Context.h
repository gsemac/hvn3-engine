#pragma once
#include "hvn3/core/IContextProvider.h"

namespace hvn3 {

	class Display;
	template<typename T>
	class ICollisionManager;
	class IGameManager;
	class IObject;
	class IObjectManager;
	class IRoom;
	class IViewManager;
	class GameProperties;
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
		IViewManager& GetViews();
		System::IContextProvider& GetContextProvider();
		GameProperties& GetProperties();
		Display& GetDisplay();
		IGameManager& GetGameManager();

		template <typename manager_type>
		manager_type* Get() {
			return static_cast<manager_type*>(_context_provider->GetManager(manager_type::Id()));
		}

		explicit operator bool() const;

	private:
		System::IContextProvider* _context_provider;

	};

}