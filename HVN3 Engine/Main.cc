#include "HVN3.h"
#include "test/Global.h"

class KeyListenerObject : public hvn3::Object, public hvn3::KeyboardListener {

public:
	KeyListenerObject() : hvn3::Object(0) {}

	void OnKeyPressed(hvn3::KeyPressedEventArgs& e) override {

		hvn3::Console::WriteLine(e.KeyCode(), " Pressed!");

	}
	void OnKeyUp(hvn3::KeyUpEventArgs& e) override {

		hvn3::Console::WriteLine(e.KeyCode(), " Released!");

	}
	void OnKeyDown(hvn3::KeyDownEventArgs& e) override {

		hvn3::Console::WriteLine(e.KeyCode(), " Held!");

	}
	void OnKeyChar(hvn3::KeyCharEventArgs& e) override {

		hvn3::Console::WriteLine(e.Char(), " Typed!");

	}

};

class MouseListenerObject : public hvn3::Object, public hvn3::MouseListener {

public:
	MouseListenerObject() : hvn3::Object(1) {}

	void OnMousePressed(hvn3::MousePressedEventArgs& e) override {

		hvn3::Console::WriteLine((int)e.Button(), " clicked ", e.Clicks(), " time(s)!");

	}
	void OnMouseDown(hvn3::MouseDownEventArgs& e) override {

		hvn3::Console::WriteLine((int)e.Button(), " held!");

	}

};

int main(int argc, char *argv[]) {

	// Initialize game properties.
	GameState.Initialize(argc, argv);
	GameState.Properties().DebugMode = true;
	GameState.Properties().Fps = 60.0f;
	GameState.Properties().FixedFrameRate = true;

	auto room = hvn3::RoomPtr(new hvn3::Room(0, GameState.Properties().DisplaySize));
	room->Objects()->AddInstance(hvn3::ObjectPtr(new KeyListenerObject));
	room->Objects()->AddInstance(hvn3::ObjectPtr(new MouseListenerObject));

	GameState.Rooms().AddRoom(room);

	// Run the main game loop.
	GameState.Loop();

	return 0;

}