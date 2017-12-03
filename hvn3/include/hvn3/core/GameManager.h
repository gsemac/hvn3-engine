#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/core/Framework.h"
#include "hvn3/rooms/RoomManager.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/objects/IObjectManager.h"

namespace hvn3 {

	class GameManager {

	public:
		GameManager();
		GameManager(int argc, char* argv[]);
		~GameManager();

		virtual void Initialize(int argc, char* argv[]);
		virtual void Loop();
		virtual void Shutdown();

		System::Properties& Properties();
		System::Runner& Runner();
		RoomManager& Rooms();
		RoomManager::room_type::collision_manager_type& Collisions();
		IObjectManager& Objects();

	private:
		System::Properties _properties;
		System::Runner* _runner;
		hvn3::RoomManager _room_manager;

		void _onShutdown();

	};

}