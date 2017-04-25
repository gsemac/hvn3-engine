#include "HVN3.h"

Game MyGame;

class Ball : public Object {

public:
	Ball(float x, float y) : Object(x, y) {
		std::cout << "Ball created at " << x << ", " << y << "!\n";

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

		if (Keyboard::KeyPressed(ALLEGRO_KEY_F5))
			MyGame.RoomManager().LoadNext();

	}

private:
	float _radius;

};

class TestRoom : public Room {

public:
	TestRoom() : Room(640, 480) {}

	void SetUp() override {

		SetBackgroundColor(Color::DarkGrey);

		for (int i = 0; i < 100; ++i)
			ObjectManager().InstanceAdd(Object::Create<Ball>(200, 200));

	}

private:


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

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}