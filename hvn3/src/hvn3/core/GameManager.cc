#include "hvn3/core/GameManager.h"
#include "hvn3/core/Framework.h"
#include "hvn3/core/Runner.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Bitmap.h"

namespace hvn3 {

	GameManager::GameManager() :
		_room_manager(Context()) {

		// Initialize the underlying framework.
		System::Framework::Initialize();

		// Set new bitmap flags.
		Graphics::Bitmap::SetNewBitmapFlags(Graphics::BitmapFlags::AllegroDefault | Graphics::BitmapFlags::MagLinear | Graphics::BitmapFlags::MinLinear);

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
			_runner = new System::Runner(Context());

		// Execute the main game loop.
		_runner->Loop();

	}
	void GameManager::Shutdown() {

		_onShutdown();

	}

	System::Properties& GameManager::GetGameProperties() {
		return _properties;
	}
	System::Runner& GameManager::GetRunner() {
		return *_runner;
	}

	hvn3::Context GameManager::Context() {
		return hvn3::Context(this);
	}



	IGameManager& GameManager::GetGameManager() {
		return *this;
	}
	RoomManager& GameManager::GetRoomManager() {
		return _room_manager;
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