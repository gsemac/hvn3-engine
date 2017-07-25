#include "HVN3.h"
#include "test/Global.h"

class KeyListenerObject : 
	public hvn3::Object, 
	public hvn3::KeyboardEventListener {

public:
	KeyListenerObject() : 
		hvn3::Object(0) {}

	void OnKeyPressed(hvn3::KeyPressedEventArgs& e) override {

		hvn3::Console::WriteLine(e.KeyCode(), " Pressed!");

	}
	void OnKeyReleased(hvn3::KeyReleasedEventArgs& e) override {

		hvn3::Console::WriteLine(e.KeyCode(), " Released!");

	}
	void OnKeyDown(hvn3::KeyDownEventArgs& e) override {

		hvn3::Console::WriteLine(e.KeyCode(), " Held!");

	}
	void OnKeyChar(hvn3::KeyCharEventArgs& e) override {

		hvn3::Console::WriteLine(e.Char(), " Typed!");

	}

};

int main(int argc, char *argv[]) {
	
	// Initialize game properties.
	GameState.Initialize(argc, argv);
	GameState.Properties().DebugMode = true;
	GameState.Properties().Fps = 60.0f;
	GameState.Properties().FixedFrameRate = true;

	auto room = hvn3::Room::Create<hvn3::Room>(0, GameState.Properties().DisplaySize);
	room->Objects()->AddInstance(KeyListenerObject::Create<KeyListenerObject>());

	GameState.Rooms().AddRoom(room);
	
	// Run the main game loop.
	GameState.Loop();

	return 0;

}