//#include "hvn3/core/GameManager.h"
//#include "hvn3/core/Engine.h"
//#include "hvn3/core/Runner.h"
//#include "hvn3/exceptions/Exception.h"
//#include "hvn3/graphics/Bitmap.h"
//#include "hvn3/utility/Environment.h"
//
//#include <cassert>
//
//namespace hvn3 {
//
//	GameManager::GameManager() :
//		GameManager(ApplicationProperties()) {
//	}
//	GameManager::GameManager(int argc, char* argv[]) :
//		GameManager(argc, argv, ApplicationProperties()) {
//	}
//	GameManager::GameManager(const ApplicationProperties& properties) :
//		_global(this, [this](IManager* m) {
//		ContextChangedEventArgs args(Context());
//		m->OnContextChanged(args);
//	}),
//		_properties(properties) {
//
//		_onInit();
//
//	}
//	GameManager::GameManager(int argc, char* argv[], const ApplicationProperties& properties) :
//		GameManager(properties) {
//	}
//	GameManager::~GameManager() {
//
//		Shutdown();
//
//	}
//
//	void GameManager::Initialize(int argc, char* argv[]) {
//
//		Environment::SetCommandLineArgs(argc, argv);
//
//	}
//	void GameManager::Loop() {
//
//		// Execute the main game loop.
//		_runner->Loop();
//
//	}
//	void GameManager::Exit() {
//		_runner->Stop();
//	}
//	void GameManager::Shutdown() {
//
//		_onShutdown();
//
//	}
//
//	ApplicationProperties& GameManager::Properties() {
//		return _properties;
//	}
//	System::Runner& GameManager::Runner() {
//		return *_runner;
//	}
//	Display& GameManager::Display() {
//		return *_display;
//	}
//
//	hvn3::ApplicationContext GameManager::Context() {
//
//		hvn3::ApplicationContext context(&_global, nullptr);
//
//		return context;
//
//	}
//
//	void GameManager::_onInit() {
//
//		Engine::Initialize();
//
//		_display = std::make_unique<class Display>(_properties.DisplaySize, _properties.ApplicationName, _properties.DisplayFlags);
//		_runner = std::make_unique<System::Runner>(Context());
//
//		// Enable smooth scaling of bitmaps by default.
//		// Graphics::Bitmap::SetNewBitmapFlags(Graphics::BitmapFlags::AllegroDefault | Graphics::BitmapFlags::MagLinear | Graphics::BitmapFlags::MinLinear);
//
//		// Full-screen the display if this was set in the game properties.
//		if (_properties.StartFullscreen)
//			_display->SetFullscreen(true);
//
//		// Hide the cursor if this was set in the game properties.
//		if (!_properties.DisplayCursor)
//			_display->SetCursorVisible(false);
//
//		_global.Register<ROOM_MANAGER>(std::make_unique<RoomManager>());
//
//	}
//	void GameManager::_onShutdown() {
//
//		// Call the destructors for all members that could possibly be hanging onto to framework objects.
//		// There should be a better way of dealing with this (reference counting?).
//		_global.Get<ROOM_MANAGER>().~IRoomManager();
//		_display.release();
//		_runner.release();
//
//		// Shutdown the framework. At this point, all other framework objects should be deinitialized (bitmaps, etc.).
//		Engine::Deinitialize();
//
//	}
//
//}