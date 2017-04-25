#include "HVN3.h"
//#include "ExampleGame.h"
#include "ExampleGame2.h"
#include "Display.h"
#include "Graphics.h"
#include "Tileset.h"
#include "RoomManager.h"
#include "ObjectManager.h"
#include "UpdateEventArgs.h"
#include "Random.h"

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

	Framework framework(argc, argv);

	Properties properties;
	properties.DisplayTitle = "Hello, world!";
	properties.OutsideColor = Color::Black;
	properties.DebugMode = true;
	properties.FPS = 60;

	RoomManager room_manager;
	room_manager.RoomAdd(Room::Create<TestRoom>());

	//ObjectManager obj_manager(std::unique_ptr<ICollisionManager>(new CollisionManager(std::unique_ptr<IBroadPhaseCollisionManager>(new CollisionGrid(32, 32)))));

	//getchar();

	// Run the main game loop.
	framework.Loop(Runner(properties, room_manager));

	return 0;

}