#pragma once
#include "Framework.h"
#include "RoomManager.h"
#include "Runner.h"
#include "ResourceManager.h"

namespace hvn3 {

	class GameManager {

	public:
		GameManager();
		virtual ~GameManager();
		virtual void Initialize(int argc, char* argv[]);
		virtual void Loop();

		Properties& Properties();
		RoomManager& RoomManager();
		ResourceManager& ResourceManager();
		Runner& Runner();

	protected:
		virtual void Shutdown();

	private:
		hvn3::Properties _properties;
		hvn3::RoomManager _room_manager;
		hvn3::ResourceManager _resource_manager;
		hvn3::Runner* _runner;

	};

}