#define HVN3_DEBUG
#include "HVN3.h"
#include "LevelEditor.h"
using namespace hvn3;
using namespace hvn3::Gui;

GameManager MyGame;

class MyRoom : public Room {

public:
	MyRoom(float width, float height) :
		Room(width, height),
		_gui_manager(Rectangle(MyGame.Properties().DisplaySize.Width(), MyGame.Properties().DisplaySize.Height())) {
	}
	~MyRoom() {

		std::cout << "room destructed";

	}

	void OnUpdate(UpdateEventArgs& e) override {

		Room::OnUpdate(e);

		_gui_manager.OnUpdate(e);

	}
	void OnDraw(DrawEventArgs& e) override {

		Room::OnDraw(e);

	}
	void Render(DrawEventArgs& e) override {

		Room::Render(e);

		_gui_manager.OnDraw(e);

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

	void OnUpdate(UpdateEventArgs& e) override {

		if (Keyboard::KeyPressed(ALLEGRO_KEY_F5))
			MyGame.RoomManager().LoadNext();

		if (Keyboard::KeyPressed(ALLEGRO_KEY_O)) {
			std::cout << MyGame.ResourceManager().Tilesets()[TILESET_1]->TileCount();
		}

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawBitmap(0, 0, &MyGame.ResourceManager().Tilesets()[TILESET_1]->TileAt(5));

		e.Graphics().DrawCircle(Mouse::Position(), 10, Color::AliceBlue, 1);

	}

};

class oBall : public Object {

public:
	oBall(float x, float y) : Object(x, y) {

		_radius = Random::Float(10, 25);
		Velocity() = Vector2d(Random::Float(0, 360), Random::Float(0.1, 1));

	}

	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(Point(X() + 2, Y() + 2), _radius, Color(0, 0, 0, 0.2), 2);
		e.Graphics().DrawCircle(Point(X(), Y()), _radius, Color::LtGrey, 2);

	}
	void OnUpdate(UpdateEventArgs& e) override {

		Object::OnUpdate(e);

		float r = _radius;
		Room* room = MyGame.RoomManager().CurrentRoom();

		if ((Y() - r < 0 && Velocity().Y() < 0) || (Y() + r > room->Height() && Velocity().Y() > 0))
			Velocity().SetY(Velocity().Y() * -1);

		if ((X() - r < 0 && Velocity().X() < 0) || (X() + r > room->Width() && Velocity().X() > 0))
			Velocity().SetX(Velocity().X() * -1);

	}
	void Collide(ICollidable* other) override {



	}

private:
	float _radius;

};

class ScrollBox : public ScrollableControl {

public:
	ScrollBox(const Point& p, const Size& s) :
		ScrollableControl(Size(MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]->Width(), MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]->Height())),
		Control(p, s) {
	}

	void OnPaint(PaintEventArgs& e) override {

		e.Graphics().Clear(BackColor());

		e.Graphics().DrawBitmap(-ScrollPosition().X(), -ScrollPosition().Y(), &MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]->Bitmap());

		ScrollableControl::OnPaint(e);

	}

};

class TestRoom : public MyRoom {

public:
	TestRoom() : MyRoom(960, 720) {

		SetPersistent(true);

	}

	void SetUp() override {

		SetBackgroundColor(Color::Silver);

		BackgroundManager()->BackgroundAdd(MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]);
		BackgroundManager()->PropertiesAt(0).SetTiledHorizontally(true);
		BackgroundManager()->PropertiesAt(0).SetVelocity(Vector2d(0, 1));

		ObjectManager()->InstanceAdd(Object::Create<oController>());

		for (int i = 0; i < 100; ++i)
			ObjectManager()->InstanceAdd(Object::Create<oBall>(200, 200));
		

	}
	void OnRoomEnter(RoomEnterEventArgs& e) override {

		std::cout << "Entering room " << this << " with " << ObjectManager()->InstanceCount() << " instances\n";

	}
	void OnRoomExit(RoomExitEventArgs& e) override {

		std::cout << "Exiting room " << this << " with " << ObjectManager()->InstanceCount() << " instances\n";

	}
	void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override {

		Resize(e.NewSize().Width(), e.NewSize().Height());
		std::cout << "size changed\n";
		//ViewManager()->ViewRemove(0);

		//ViewManager()->ViewAt(0) = View(Point(0, 0), Size(e.NewSize().Width(), e.NewSize().Height()), Point(0, 0), Size(e.NewSize().Width(), e.NewSize().Height()));

	}

private:
	void CreateGuiComponents() {

		// Create the window.
		Size wind_size(MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]->Width(), MyGame.ResourceManager().Backgrounds()[BACKGROUND_1]->Height());
		Window* wind = new Window(50, 50, wind_size.Width(), wind_size.Height(), "My Window");
		wind->SetMaximumSize(Size(wind_size.Width(), wind_size.Height() + wind->TitlebarHeight()));
		ScrollBox* scrollbox = new ScrollBox(Point(0, 0), Size(wind->Width(), wind->Height() - wind->TitlebarHeight()));
		scrollbox->SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_BOTTOM);
		wind->Controls()->AddControl(Control::Create(scrollbox));
		wind->SetOpacity(1.0f);

		// Create the toolstrip.
		ToolStrip* toolstrip = new ToolStrip;
		ToolStripDropDown* dropdown = new ToolStripDropDown(Point(300, 300), 200);
		dropdown->AddItem(new ToolStripMenuItem);
		dropdown->AddItem(new ToolStripMenuItem);

		// Add everything to the gui manager.
		GuiManager()->ControlManager()->AddControl(Control::Create(toolstrip));
		GuiManager()->ControlManager()->AddControl(Control::Create(dropdown));

	}

};

int main(int argc, char *argv[]) {

	// Initialize game properties.
	MyGame.Initialize(argc, argv);
	MyGame.Properties().DebugMode = false;
	MyGame.Properties().OutsideColor = Color::Black;
	MyGame.Properties().DisplaySize = Size(960, 720);
	MyGame.Properties().DisplayFlags |= DisplayFlags::Resizable;
	MyGame.Properties().ScalingMode = ScalingMode::MaintainAspectRatio;
	MyGame.Properties().StartFullscreen = false;
	MyGame.Properties().FPS = 60;

	// Load resources.
	MyGame.ResourceManager().Tilesets().Add(TILESET_1, Resource::Create<Tileset>("data/test/tileset1.png", 32, 32));
	MyGame.ResourceManager().Backgrounds().Add(BACKGROUND_1, Resource::Create<Background>("data/test/tileset1.png"));

	// Set up the first scene.
	Size large_size(MyGame.Properties().DisplaySize.Width() * 2, MyGame.Properties().DisplaySize.Height() * 2);
	Size medium_size(1000, 800);
	Size small_size(640, 480);
	MyGame.RoomManager().AddRoom(Room::Create<editor::LevelEditor>(medium_size));
	MyGame.RoomManager().AddRoom(Room::Create<TestRoom>());
	MyGame.RoomManager().SetRoomTransition<RoomTransitionFade>(Color::Black, true);

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}