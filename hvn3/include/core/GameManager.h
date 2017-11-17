#pragma once
#include "graphics/Color.h"
#include "core/Framework.h"
#include "rooms/RoomManager.h"
#include "assets/ResourceManager.h"
#include "collision/ICollisionManager.h"
#include "objects/IObjectManager.h"

namespace hvn3 {

	class GameManager {

	public:
		GameManager();
		GameManager(int argc, char* argv[]);

		virtual void Initialize(int argc, char* argv[]);
		virtual void Loop();

		System::Properties& Properties();
		System::Runner& Runner();
		RoomManager& Rooms();
		ResourceManager& Resources();
		RoomManager::room_type::collision_manager_type& Collisions();
		IObjectManager& Objects();

	protected:
		virtual void _shutdown();

	private:
		System::Properties _properties;
		System::Runner* _runner;
		hvn3::RoomManager* _room_manager;
		hvn3::ResourceManager _resource_manager;

	};

}