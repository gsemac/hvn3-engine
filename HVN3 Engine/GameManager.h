#pragma once
#include "Framework.h"
#include "RoomManager.h"
#include "Runner.h"
#include "ResourceManager.h"
#include "ICollisionManager.h"
#include "IObjectManager.h"

namespace hvn3 {

	class GameManager {

	public:
		GameManager();
		GameManager(int argc, char* argv[]);

		virtual void Initialize(int argc, char* argv[]);
		virtual void Loop();

		System::Properties* Properties();
		RoomManager* Rooms();
		ResourceManager* Resources();
		Collision::ICollisionManager<Object*>* Collisions();
		IObjectManager* Objects();
		System::Runner* Runner();

	protected:
		virtual void _shutdown();

	private:
		System::Properties _properties;
		hvn3::RoomManager* _room_manager;
		hvn3::ResourceManager _resource_manager;
		System::Runner* _runner;

	};

}