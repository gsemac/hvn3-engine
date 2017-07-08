#include "GameManager.h"
#include "Bitmap.h"
#include "Exception.h"

namespace hvn3 {

	GameManager::GameManager() {

		Framework::Initialize();

		// Set new bitmap flags.
		Drawing::SetNewBitmapFlags(hvn3::Drawing::BitmapFlags::Default | hvn3::Drawing::BitmapFlags::MagLinear | hvn3::Drawing::BitmapFlags::MinLinear);

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
	ICollisionManager<Object*>& GameManager::Collisions() {

		auto room_ptr = _room_manager.CurrentRoom();

		if(room_ptr == nullptr)
			throw System::NotSupportedException("Attempted to access the collision manager for the current room, but there is no active room.");

		auto ptr = room_ptr->Collisions();

		if (ptr == nullptr)
			throw System::NotSupportedException("The current room does not provide a collision manager.");

		return *ptr;

	}
	IObjectManager& GameManager::Objects() {
		
		auto room_ptr = _room_manager.CurrentRoom();

		if (room_ptr == nullptr)
			throw System::NotSupportedException("Attempted to access the object manager for the current room, but there is no active room.");
		
		auto ptr = room_ptr->Objects();

		if (ptr == nullptr)
			throw System::NotSupportedException("The current room does not provide an object manager.");

		return *ptr;

	}
	Runner& GameManager::Runner() {

		return *_runner;

	}

}