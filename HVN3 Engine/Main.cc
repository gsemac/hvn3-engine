#include "HVN3.h"
using namespace Gui;

GameManager MyGame;

class MyRoom : public Room {

public:
	MyRoom(float width, float height) :
		Room(width, height) {
	}
	~MyRoom() {

		std::cout << "room destructed";

	}

	void Update(UpdateEventArgs& e) override {

		Room::Update(e);

		_gui_manager.Update(e);

	}
	void Draw(DrawEventArgs& e) override {

		Room::Draw(e);

		_gui_manager.Draw(e);

	}

	Gui::GuiManager* GuiManager() {

		return &_gui_manager;

	}

protected:
	void Reset() override {

		Room::Reset();

		_gui_manager.Clear();

	}

private:
	Gui::GuiManager _gui_manager;

};

enum MyResources : ResourceId {
	TILESET_1,
	BACKGROUND_1
};

class oController : public Object {

public:
	oController() : Object(0, 0) {}

	void Update(UpdateEventArgs& e) override {

		if (Keyboard::KeyPressed(ALLEGRO_KEY_F5))
			MyGame.RoomManager().LoadNext();

		if (Keyboard::KeyPressed(ALLEGRO_KEY_O)) {
			std::cout << MyGame.ResourceManager().Tilesets()[TILESET_1]->TileCount();
		}

	}
	void Draw(DrawEventArgs& e) override {

		e.Graphics().DrawBitmap(0, 0, MyGame.ResourceManager().Tilesets()[TILESET_1]->TileAt(5));

		e.Graphics().DrawCircle(Mouse::Position(), 5, Color::AliceBlue, 1);

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

	}
	void Update(UpdateEventArgs& e) override {

		Object::Update(e);

		float r = _radius;

		if ((Y() - r < 0 && Velocity().Y() < 0) || (Y() + r > 480 && Velocity().Y() > 0))
			Velocity().SetY(Velocity().Y() * -1);

		if ((X() - r < 0 && Velocity().X() < 0) || (X() + r > 640 && Velocity().X() > 0))
			Velocity().SetX(Velocity().X() * -1);

	}
	void Collide(ICollidable* other) override {



	}

private:
	float _radius;

};

class TestRoom : public MyRoom {

public:
	TestRoom() : MyRoom(640, 480) {

		SetPersistent(true);

	}

	void SetUp() override {

		SetBackgroundColor(Color::DarkGrey);

		BackgroundManager()->BackgroundAdd(MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]);
		BackgroundManager()->PropertiesAt(0).SetTiledHorizontally(true);
		BackgroundManager()->PropertiesAt(0).SetVelocity(Vector2d(0, 1));

		ObjectManager()->InstanceAdd(Object::Create<oController>());

		for (int i = 0; i < 100; ++i)
			ObjectManager()->InstanceAdd(Object::Create<oBall>(200, 200));

		//Window* wind = new Window(0, 0, 300, 400, "Window 1");
		//wind->Controls()->AddControl(Control::Create<Button>(100, 100, 150, 25, "Window Button 1"));
		//wind->Controls()->AddControl(Control::Create<Textbox>(100, 200, 150, 25));
		//wind->Controls()->AddControl(Control::Create<Window>(100, 100, 100, 100, "Window 2"));

		//GuiManager()->ControlManager()->AddControl(Control::Create(wind));
		//GuiManager()->ControlManager()->AddControl(Control::Create<Window>(100, 100, 100, 100, "Window 2"))->SetBackColor(Color::DodgerBlue);
		Panel* panel = new Panel(100, 100, 100, 100);
		panel->Controls()->AddControl(Control::Create<Button>(10, 10, 120, 25, "Panel Button"));
		GuiManager()->ControlManager()->AddControl(Control::Create<Panel>(panel));
		GuiManager()->ControlManager()->AddControl(Control::Create<Scrollbar>(nullptr, Point(0, 0), Size(3, 480)));

	}
	void OnRoomEnter(RoomEnterEventArgs& e) override {

		std::cout << "Entering room " << this << " with " << ObjectManager()->InstanceCount() << " instances\n";

	}
	void OnRoomExit(RoomExitEventArgs& e) override {

		std::cout << "Exiting room " << this << " with " << ObjectManager()->InstanceCount() << " instances\n";

	}

};

int main(int argc, char *argv[]) {

	// Initialize game properties.
	MyGame.Initialize(argc, argv);
	MyGame.Properties().DebugMode = true;
	MyGame.Properties().OutsideColor = Color::Black;

	// Load resources.
	MyGame.ResourceManager().Tilesets().Add(TILESET_1, Resource::Create<Tileset>("data/test/tileset1.png", 32, 32));
	MyGame.ResourceManager().Backgrounds().Add(BACKGROUND_1, Resource::Create<Background>("data/test/tileset1.png"));

	// Set up the first scene.
	MyGame.RoomManager().RoomAdd(Room::Create<TestRoom>());
	MyGame.RoomManager().RoomAdd(Room::Create<TestRoom>());

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}