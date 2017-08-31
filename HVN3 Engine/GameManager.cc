#include "GameManager.h"
#include "Bitmap.h"
#include "Exception.h"

namespace hvn3 {

	GameManager::GameManager() {

		// Initialize the underlying framework.
		System::Framework::Initialize();

		// Set new bitmap flags.
		Drawing::SetNewBitmapFlags(Drawing::BitmapFlags::Default | Drawing::BitmapFlags::MagLinear | Drawing::BitmapFlags::MinLinear);

		// Create a new room manager.
		_room_manager = new RoomManager;

	}
	GameManager::GameManager(int argc, char* argv[]) : GameManager() {

		Initialize(argc, argv);

	}

	void GameManager::Initialize(int argc, char* argv[]) {

		// Initialize the underlying framework.
		System::Framework::Initialize(argc, argv);

	}
	void GameManager::Loop() {

		// Create a new runner for handling the game loop.
		// The runner will automatically create the display upon construction.
		_runner = new System::Runner(&_properties, _room_manager);

		// Execute the main game loop.
		_runner->Loop();

		// Call the shutdown routine when the loop is exited.
		_shutdown();

	}
	void GameManager::_shutdown() {

		// Delete the runner if one was created.
		if (_runner != nullptr)
			delete _runner;
		_runner = nullptr;
		
		// Delete the room manager if one was created.
		// This will trigger the exit event for the current room and free all resources used by it.
		if (_room_manager != nullptr)
			delete _room_manager;
		_room_manager = nullptr;

		// Clear all resources (this will free them automatically).
		_resource_manager.Clear();

		// Shutdown the underlying framework.
		System::Framework::Shutdown();

	}

	System::Properties& GameManager::Properties() {

		return _properties;

	}
	hvn3::RoomManager* GameManager::Rooms() {

		return _room_manager;

	}
	ResourceManager* GameManager::Resources() {

		return &_resource_manager;

	}
	Collision::ICollisionManager<Object*>* GameManager::Collisions() {

		auto room_ptr = _room_manager->CurrentRoom();

		if (room_ptr == nullptr)
			throw System::NotSupportedException("Attempted to access the collision manager for the current room, but there is no active room.");

		auto ptr = room_ptr->Collisions();

		if (ptr == nullptr)
			throw System::NotSupportedException("The current room does not provide a collision manager.");

		return ptr;

	}
	IObjectManager* GameManager::Objects() {

		auto room_ptr = _room_manager->CurrentRoom();

		if (room_ptr == nullptr)
			throw System::NotSupportedException("Attempted to access the object manager for the current room, but there is no active room.");

		auto ptr = room_ptr->Objects();

		if (ptr == nullptr)
			throw System::NotSupportedException("The current room does not provide an object manager.");

		return ptr;

	}
	System::Runner* GameManager::Runner() {

		return _runner;

	}

}