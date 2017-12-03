#include "hvn3/core/GameManager.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/exceptions/Exception.h"

namespace hvn3 {

	GameManager::GameManager() {

		// Initialize the underlying framework.
		System::Framework::Initialize();

		// Set new bitmap flags.
		Graphics::SetNewBitmapFlags(Graphics::BitmapFlags::Default | Graphics::BitmapFlags::MagLinear | Graphics::BitmapFlags::MinLinear);

	}
	GameManager::GameManager(int argc, char* argv[]) : GameManager() {

		Initialize(argc, argv);

	}
	GameManager::~GameManager() {

		Shutdown();

	}

	void GameManager::Initialize(int argc, char* argv[]) {

		// Initialize the underlying framework.
		System::Framework::Initialize(argc, argv);

	}
	void GameManager::Loop() {

		// Create a new runner for handling the game loop.
		// The runner will automatically create the display upon construction.
		if (_runner == nullptr)
			_runner = new System::Runner(&_properties, &_room_manager);

		// Execute the main game loop.
		_runner->Loop();

	}
	void GameManager::Shutdown() {

		_onShutdown();

	}

	System::Properties& GameManager::Properties() {

		return _properties;

	}
	RoomManager& GameManager::Rooms() {

		return _room_manager;

	}
	RoomManager::room_type::collision_manager_type& GameManager::Collisions() {

		auto room_ptr = _room_manager.CurrentRoom();

		if (room_ptr == nullptr)
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
	System::Runner& GameManager::Runner() {

		return *_runner;

	}



	void GameManager::_onShutdown() {

		// Delete the runner if one was created.
		if (_runner != nullptr)
			delete _runner;
		_runner = nullptr;

		// Call the destructor for the room manager so all of its resources are freed before shutting down the framework.
		_room_manager.~RoomManager();

		// Shutdown the framework. At this point, all other framework objects should be deinitialized (bitmaps, etc.).
		System::Framework::Shutdown();

	}

}