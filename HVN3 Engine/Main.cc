#define HVN3_DEBUG
#include "HVN3.h"
#include "LevelEditor.h"
#include "CollisionManager.h"
#include "CollisionGrid.h"
using namespace hvn3;
using namespace hvn3::Gui;

GameManager MyGame;

class MyRoom : public Room {

public:
	MyRoom(float width, float height) :
		Room(0, SizeI(width, height)),
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
			MyGame.Rooms().GotoRoom(MyGame.Rooms().CurrentRoom()->Id());

		if (Keyboard::KeyPressed(ALLEGRO_KEY_O)) {
			std::cout << MyGame.Resources().Tilesets()[TILESET_1]->TileCount();
		}

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawBitmap(0, 0, &MyGame.Resources().Tilesets()[TILESET_1]->TileAt(5));

		CollisionManager* cm = (CollisionManager*)MyGame.Rooms().CurrentRoom()->Objects().CollisionManager().get();
		CollisionGrid& cg = (CollisionGrid&)cm->BroadPhase();

		cg.DrawCells(e);

	}

};

class oBall : public Object {

public:
	oBall(float x, float y) :
		Object(1, PointF(x, y)),
		_hit_mask(CircleF(10.0f)) {

		_radius = Random::Float(10, 25);
		_hit_mask = CircleHitMask(_radius);
		_velocity = Vector2d(Random::Float(0, 360), Random::Float(0.1, 1));

		Collider().SetHitMask(&_hit_mask);
		Collider().Filter().SetCategoryBits(0b010);
		Collider().Filter().SetMaskBits(0b010);

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
			_velocity.SetY(-_velocity.Y());

		if ((X() - r < 0 && _velocity.X() < 0) || (X() + r > room->Width() && _velocity.X() > 0))
			_velocity.SetX(-_velocity.X());

		SetPosition(Position() + _velocity);

		// Slowdown.
		_velocity *= 0.99f;

	}
	void OnCollision(CollisionEventArgs& e) override {

		MyGame.Rooms().CurrentRoom()->Objects().CollisionManager()->MoveOutsideObject(this, e.Other(), 
			PointDirection(e.Other()->Position(), Position()),
			1.0f
		);

		_velocity = -_velocity;

	}

	void SetVelocity(const Vector2d& vec) {

		_velocity = vec;

	}
	const Vector2d& Velocity() const {

		return _velocity;

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

		_last_position = Position();

	}

	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(Position(), 10, Color::AliceBlue, 1);

	}
	void OnUpdate(UpdateEventArgs& e) override {

		_last_position = Position();

		SetPosition(Mouse::Position());

	}
	void OnCollision(CollisionEventArgs& e) override {

		switch (e.Other()->Id()) {

		case 1: {

			oBall* ptr = (oBall*)e.Other();

			Vector2d move_vec(_last_position, Position());
			Vector2d dir_vec = Vector2d(Position(), ptr->Position()) + move_vec;

			Vector2d tot_vec(move_vec.Angle(), dir_vec.Magnitude());

			// Set the ball's new velocity.	
			ptr->SetVelocity(tot_vec);

			// Move the ball outside of the cursor.
			if (tot_vec.Magnitude() > 0)
				MyGame.Rooms().CurrentRoom()->Objects().CollisionManager()->MoveOutsideObject(e.Other(), this, PointDirection(e.Other()->Position(), Position()), 1);

			break;

		}
		}

	}

private:
	CircleHitMask _hit_mask;
	PointF _last_position;

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

		Backgrounds().BackgroundAdd(MyGame.Resources().Backgrounds(BACKGROUND_1));
		Backgrounds().PropertiesAt(0).SetTiledHorizontally(true);
		Backgrounds().PropertiesAt(0).SetVelocity(Vector2d(0, 1));

		Objects().AddInstance(Object::Create<oController>());
		Objects().AddInstance(Object::Create<oMouseBox>());

		for (int i = 0; i < 100; ++i)
			Objects().AddInstance(Object::Create<oBall>(Random::Float(Width()), Random::Float(Height())));


	}
	void OnRoomEnter(RoomEnterEventArgs& e) override {

		std::cout << "Entering room " << this << " with " << Objects().InstanceCount() << " instances\n";

	}
	void OnRoomExit(RoomExitEventArgs& e) override {

		std::cout << "Exiting room " << this << " with " << Objects().InstanceCount() << " instances\n";

	}
	void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) override {

		//Resize(e.NewSize().Width(), e.NewSize().Height());
		//std::cout << "size changed\n";
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
	MyGame.Properties().ScalingMode = ScalingMode::Fixed;
	MyGame.Properties().StartFullscreen = false;
	MyGame.Properties().FPS = 60;

	// Load resources.
	MyGame.Resources().Tilesets().Add(TILESET_1, Resource::Create<Tileset>("data/test/tileset1.png", SizeI(32, 32)));
	MyGame.Resources().Backgrounds().Add(BACKGROUND_1, Resource::Create<Background>("data/test/tileset1.png"));
	MyGame.Resources().Sprites().Add(BACKGROUND_1, Sprite::Create("data/test/tileset1.png"));

	// Set up the first scene.
	SizeI large_size(MyGame.Properties().DisplaySize.Width() * 2, MyGame.Properties().DisplaySize.Height() * 2);
	SizeI medium_size(1000, 800);
	SizeI small_size(640, 480);
	MyGame.Rooms().AddRoom(Room::Create<editor::LevelEditor>(large_size, MyGame.Properties().DisplaySize));
	//MyGame.Rooms().AddRoom(Room::Create<TestRoom>());
	MyGame.Rooms().SetRoomTransition(RoomTransitionFade::Create(Color::Black, true));

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}