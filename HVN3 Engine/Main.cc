#include "HVN3.h"

Game MyGame;

class oController : public Object {

public:
	oController() : Object(0, 0) {}
	
	void Update(UpdateEventArgs& e) override {

		if (Keyboard::KeyPressed(ALLEGRO_KEY_F5))
			MyGame.RoomManager().LoadNext();

	}

};

class oBall : public Object {

public:
	oBall(float x, float y) : Object(x, y) {

		_radius = Random::Float(10, 25);
		Velocity() = Vector2d(Random::Float(0, 360), Random::Float(0.1, 1));

	}

	void Draw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(Point(X() + 2, Y() + 2), _radius, Color(0, 0, 0, 0.2), 2);
		e.Graphics().DrawCircle(Point(X(), Y()), _radius, Color::LtGrey, 2);

		e.Graphics().DrawCircle(Mouse::Position(), 5, Color::AliceBlue, 1);

	}
	void Update(UpdateEventArgs& e) override {
		
		Object::Update(e);

		float r = _radius;

		if ((Y() - r < 0 && Velocity().Y() < 0) || (Y() + r > 480 && Velocity().Y() > 0))
			Velocity().SetY(Velocity().Y() * -1);

		if ((X() - r < 0 && Velocity().X() < 0) || (X() + r > 640 && Velocity().X() > 0))
			Velocity().SetX(Velocity().X() * -1);

	}

private:
	float _radius;

};

class TestRoom : public Room {

public:
	TestRoom() : Room(640, 480) {
	
		SetPersistent(false);
		_room_id = Random::Integer();

	}

	void SetUp() override {

		SetBackgroundColor(Color::DarkGrey);
		
		ObjectManager().InstanceAdd(Object::Create<oController>());

		for (int i = 0; i < 100; ++i)
			ObjectManager().InstanceAdd(Object::Create<oBall>(200, 200));

	}
	void OnRoomEnter(RoomEnterEventArgs& e) override {

		std::cout << "Entering room " << _room_id << " with " << ObjectManager().InstanceCount() << " instances\n";

	}
	void OnRoomExit(RoomExitEventArgs& e) override {

		std::cout << "Exiting room " << _room_id << " with " << ObjectManager().InstanceCount() << " instances\n";

	}

private:
	RoomId _room_id;

};

int main(int argc, char *argv[]) {

	// Initialize game properties.
	MyGame.Initialize(argc, argv);
	MyGame.Properties().DisplayTitle = "Hello, world!";
	MyGame.Properties().OutsideColor = Color::Black;
	MyGame.Properties().DebugMode = true;
	MyGame.Properties().FPS = 60;

	// Set up the first scene.
	MyGame.RoomManager().RoomAdd(Room::Create<TestRoom>());
	MyGame.RoomManager().RoomAdd(Room::Create<TestRoom>());

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}