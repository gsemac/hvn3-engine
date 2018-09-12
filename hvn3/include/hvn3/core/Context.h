#pragma once
#include "hvn3/core/IContextProvider.h"

namespace hvn3 {

	class Display;
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

		IObjectManager& Objects();
		IRoom& Room();
		RoomManager& Rooms();
		ICollisionManager& Collisions();
		Physics::IPhysicsManager& Physics();
		IViewManager& Views();
		System::IContextProvider& ContextProvider();
		GameProperties& Properties();
		Display& Display();
		IGameManager& GameManager();

		template <typename manager_type>
		manager_type* Get() {
			return static_cast<manager_type*>(_context_provider->GetManager(manager_type::Id()));
		}

		explicit operator bool() const;

	private:
		System::IContextProvider* _context_provider;

	};

}