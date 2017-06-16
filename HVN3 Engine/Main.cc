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
		_gui_manager(RectangleF(MyGame.Properties().DisplaySize.Width(), MyGame.Properties().DisplaySize.Height())) {
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
	oController() : Object(noone, PointF(0, 0)) {}

	void OnUpdate(UpdateEventArgs& e) override {

		if (Keyboard::KeyPressed(ALLEGRO_KEY_F5))
			MyGame.Rooms().LoadNext();

		if (Keyboard::KeyPressed(ALLEGRO_KEY_O)) {
			std::cout << MyGame.Resources().Tilesets()[TILESET_1]->TileCount();
		}

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawBitmap(0, 0, &MyGame.Resources().Tilesets()[TILESET_1]->TileAt(5));

	}

};

class oBall : public Object {

public:
	oBall(float x, float y) :
		Object(1, PointF(0, 0)),
		_hit_mask(CircleF(10.0f)) {

		_radius = Random::Float(10, 25);
		_hit_mask = CircleHitMask(_radius);
		_velocity = Vector2d(Random::Float(0, 360), Random::Float(0.1, 1));

		Collider().SetHitMask(&_hit_mask);
		Collider().Filter().SetCategoryBits(0b010);
		Collider().Filter().SetMaskBits(0b000);

	}

	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(PointF(X() + 2, Y() + 2), _radius, Color::FromArgbf(0, 0, 0, 0.2), 2);
		e.Graphics().DrawCircle(PointF(X(), Y()), _radius, Color::LtGrey, 2);

	}
	void OnUpdate(UpdateEventArgs& e) override {

		Object::OnUpdate(e);

		float r = _radius;
		Room* room = MyGame.Rooms().CurrentRoom();

		if ((Y() - r < 0 && _velocity.Y() < 0) || (Y() + r > room->Height() && _velocity.Y() > 0))
			BounceV();

		if ((X() - r < 0 && _velocity.X() < 0) || (X() + r > room->Width() && _velocity.X() > 0))
			BounceH();

		SetPosition(Position() + _velocity);

	}
	void OnCollision(CollisionEventArgs& e) override {

		//std::cout << "hit!";

	}

	void BounceV() {
		_velocity.SetY(_velocity.Y() * -1);
	}
	void BounceH() {
		_velocity.SetX(_velocity.X() * -1);
	}

private:
	float _radius;
	Vector2d _velocity;
	CircleHitMask _hit_mask;

};

class oMouseBox : public Object {

public:
	oMouseBox() :
		Object(0, Mouse::Position()),
		_hit_mask(CircleF(10.0f)) {

		Collider().SetHitMask(&_hit_mask);
		Collider().Filter().SetCategoryBits(0b001);
		Collider().Filter().SetMaskBits(0b010);

	}

	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(Position(), 10, Color::AliceBlue, 1);

	}
	void OnUpdate(UpdateEventArgs& e) override {

		SetPosition(Mouse::Position());

	}
	void OnCollision(CollisionEventArgs& e) override {

		switch (e.Other().Id()) {

		case 1:
			((oBall*)&e.Other())->BounceV();
			break;

		}

	}

private:
	CircleHitMask _hit_mask;

};

class oSpriteableObject : public Object, public SpriteableBase {

public:
	oSpriteableObject() :
		Object(noone, PointF()),
		SpriteableBase(MyGame.Resources().Sprites(BACKGROUND_1)) {

		SetX(5);

	}

};

class ScrollBox : public ScrollableControl {

public:
	ScrollBox(const PointF& p, const SizeF& s) :
		ScrollableControl(SizeF(MyGame.Resources().Backgrounds()[BACKGROUND_1]->Width(), MyGame.Resources().Backgrounds()[BACKGROUND_1]->Height())),
		Control(p, s) {
	}

	void OnPaint(PaintEventArgs& e) override {

		e.Graphics().Clear(BackColor());

		e.Graphics().DrawBitmap(-ScrollPosition().X(), -ScrollPosition().Y(), &MyGame.Resources().Backgrounds()[BACKGROUND_1]->Bitmap());

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

		BackgroundManager()->BackgroundAdd(MyGame.Resources().Backgrounds()[BACKGROUND_1]);
		BackgroundManager()->PropertiesAt(0).SetTiledHorizontally(true);
		BackgroundManager()->PropertiesAt(0).SetVelocity(Vector2d(0, 1));

		ObjectManager()->InstanceAdd(Object::Create<oController>());
		ObjectManager()->InstanceAdd(Object::Create<oMouseBox>());

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
		SizeF wind_size(MyGame.Resources().Backgrounds()[BACKGROUND_1]->Width(), MyGame.Resources().Backgrounds()[BACKGROUND_1]->Height());
		Window* wind = new Window(50, 50, wind_size.Width(), wind_size.Height(), "My Window");
		wind->SetMaximumSize(SizeF(wind_size.Width(), wind_size.Height() + wind->TitlebarHeight()));
		ScrollBox* scrollbox = new ScrollBox(PointF(0, 0), SizeF(wind->Width(), wind->Height() - wind->TitlebarHeight()));
		scrollbox->SetAnchors(ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_BOTTOM);
		wind->Controls()->AddControl(Control::Create(scrollbox));
		wind->SetOpacity(1.0f);

		// Create the toolstrip.
		ToolStrip* toolstrip = new ToolStrip;
		ToolStripDropDown* dropdown = new ToolStripDropDown(PointF(300, 300), 200);
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
	MyGame.Properties().DebugMode = true;
	MyGame.Properties().OutsideColor = Color::Black;
	MyGame.Properties().DisplaySize = SizeI(960, 720);
	MyGame.Properties().DisplayFlags |= DisplayFlags::Resizable;
	MyGame.Properties().ScalingMode = ScalingMode::MaintainAspectRatio;
	MyGame.Properties().StartFullscreen = false;
	MyGame.Properties().FPS = 60;

	// Load resources.
	MyGame.Resources().Tilesets().Add(TILESET_1, Resource::Create<Tileset>("data/test/tileset1.png", 32, 32));
	MyGame.Resources().Backgrounds().Add(BACKGROUND_1, Resource::Create<Background>("data/test/tileset1.png"));
	MyGame.Resources().Sprites().Add(BACKGROUND_1, Sprite::Create("data/test/tileset1.png"));

	// Set up the first scene.
	SizeF large_size(MyGame.Properties().DisplaySize.Width() * 2, MyGame.Properties().DisplaySize.Height() * 2);
	SizeF medium_size(1000, 800);
	SizeF small_size(640, 480);
	MyGame.Rooms().AddRoom(Room::Create<TestRoom>());
	MyGame.Rooms().AddRoom(Room::Create<editor::LevelEditor>(medium_size));
	MyGame.Rooms().SetRoomTransition<RoomTransitionFade>(Color::Black, true);

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}