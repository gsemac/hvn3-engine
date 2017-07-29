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

		//hvn3::Console::WriteLine(e.KeyCode(), " Held!");

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

		//hvn3::Console::WriteLine((int)e.Button(), " held!");

	}

};

class TangentTest : public hvn3::Object {

public:
	TangentTest() : hvn3::Object(0),
		a(200.0f, 200.0f, 100.0f),
		b(250.0f, 200.0f, 50.0f)
	{

		//hvn3::Console::WriteLine("# of Common Tangents: ", hvn3::Math::Geometry::NumberOfCommonTangents(a, b));

	}

	void OnDraw(hvn3::DrawEventArgs& e) override {

		e.Graphics().DrawCircle(a, hvn3::Color::Black, 1.0f);
		e.Graphics().DrawCircle(b, hvn3::Color::Black, 1.0f);

		auto tangents = hvn3::Math::Geometry::CommonTangents(a, b);

		for (size_t i = 0; i < tangents.Count; ++i)
			e.Graphics().DrawLine(tangents.Tangents[i], i >= 2 ? hvn3::Color::Red : hvn3::Color::DodgerBlue, 1.0f);

	}
	void OnUpdate(hvn3::UpdateEventArgs& e) override {

		b = hvn3::CircleF(hvn3::Mouse::X, hvn3::Mouse::Y, b.Radius());

		hvn3::Console::SetCursorPosition(0, 0);
		hvn3::Console::WriteLine("# of Common Tangents: ", hvn3::Math::Geometry::NumberOfCommonTangents(a, b));

		//hvn3::Console::Write(hvn3::Math::Geometry::LineIntersectionPoint(2.0f, 1.0f, 2.0f, 1.0f));

		//hvn3::Math::Geometry::SlopeIntercept<float> eq = hvn3::Math::Geometry::LineSlopeIntercept(hvn3::PointF(-3, 3), hvn3::PointF(3, -1));
		//hvn3::Console::Write("y = ", eq.Slope, "x + ", eq.Intercept);

	}

private:
	hvn3::CircleF a;
	hvn3::CircleF b;


};

int main(int argc, char *argv[]) {

	// Initialize game properties.
	GameState.Initialize(argc, argv);
	GameState.Properties().DebugMode = true;
	GameState.Properties().Fps = 60.0f;
	GameState.Properties().FixedFrameRate = true;

	auto room = hvn3::RoomPtr(new hvn3::Room(0, GameState.Properties().DisplaySize));
	room->Objects()->AddInstance(hvn3::ObjectPtr(new TangentTest));

	GameState.Rooms().AddRoom(room);

	// Run the main game loop.
	GameState.Loop();

	return 0;

}