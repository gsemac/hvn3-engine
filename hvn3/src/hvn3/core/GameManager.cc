#include "hvn3/core/GameManager.h"
#include "hvn3/core/Framework.h"
#include "hvn3/core/Runner.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Bitmap.h"

namespace hvn3 {

	GameManager::GameManager() :
		GameManager(System::Properties()) {
	}
	GameManager::GameManager(int argc, char* argv[]) : GameManager() {
		Initialize(argc, argv);
	}
	GameManager::GameManager(const System::Properties& properties) :
		_properties(properties),
		_display(properties.DisplaySize, properties.DisplayTitle, properties.DisplayFlags),
		_room_manager(Context()),
		_runner(Context()) {
		_onInit();
	}
	GameManager::~GameManager() {

		Shutdown();

	}

	void GameManager::Initialize(int argc, char* argv[]) {

		// Initialize the underlying framework.
		System::Framework::Initialize(argc, argv);

	}
	void GameManager::Loop() {

		// Execute the main game loop.
		_runner.Loop();

	}
	void GameManager::Shutdown() {

		_onShutdown();

	}

	System::Properties& GameManager::GetProperties() {
		return _properties;
	}
	System::Runner& GameManager::GetRunner() {
		return _runner;
	}
	Display& GameManager::GetDisplay() {
		return _display;
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
	System::ManagerBase& GameManager::GetManagerById(System::ManagerId id) {
		return GetRoomManager().CurrentRoom()->GetManagerById(id);
		// #todo Implement for access to "global" managers.
		throw System::NotImplementedException();
	}



	void GameManager::_onInit() {

		System::Framework::Initialize();

		// Enable smooth scaling of bitmaps by default.
		// Graphics::Bitmap::SetNewBitmapFlags(Graphics::BitmapFlags::AllegroDefault | Graphics::BitmapFlags::MagLinear | Graphics::BitmapFlags::MinLinear);

		// Full-screen the display if this was set in the game properties.
		if (_properties.StartFullscreen)
			_display.SetFullscreen(true);

		// Hide the cursor if this was set in the game properties.
		if (!_properties.DisplayCursor)
			_display.SetCursorVisible(false);

	}
	void GameManager::_onShutdown() {

		// Call the destructors for all members that could possibly be hanging onto to framework objects.
		// There should be a better way of dealing with this (reference counting?).
		_room_manager.~RoomManager();
		_display.~Display();
		_runner.~Runner();

		// Shutdown the framework. At this point, all other framework objects should be deinitialized (bitmaps, etc.).
		System::Framework::Shutdown();

	}

}