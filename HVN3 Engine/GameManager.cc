#include "GameManager.h"

namespace hvn3 {

	GameManager::GameManager() {

		Framework::Initialize();

	}
	GameManager::~GameManager() {

		Shutdown();

	}
	void GameManager::Initialize(int argc, char* argv[]) {

		Framework::Initialize(argc, argv);

	}
	void GameManager::Loop() {

		_runner = new hvn3::Runner(_properties, _room_manager);

		_runner->Loop();

	}
	void GameManager::Shutdown() {

		// Delete the runner.
		delete _runner;

		// Call the destructor for the room manager to trigger the exit event for the current room.
		_room_manager.~RoomManager(); // #todo do something other than this to avoid destructing it twice
	
		// Clear all resources.
		_resource_manager.Clear();

		// Shutdown the underlying framework.
		Framework::Shutdown();

	}

	Properties& GameManager::Properties() {

		return _properties;

	}
	hvn3::RoomManager& GameManager::Rooms() {

		return _room_manager;

	}
	ResourceManager& GameManager::Resources() {

		return _resource_manager;

	}
	Runner& GameManager::Runner() {

		return *_runner;

	}

}