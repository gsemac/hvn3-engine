#include "HVN3.h"
#include "LevelEditor.h"
#include "CollisionManager.h"
#include "CollisionGrid.h"
#include "gui/Gui.h"
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
	void OnRender(DrawEventArgs& e) override {

		Room::OnRender(e);

		_gui_manager.OnDraw(e);

	}

	Gui::GuiManager* GuiManager() {

		return &_gui_manager;

	}

protected:
	void OnReset() override {

		Room::OnReset();

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
	oController() : Object(NoOne, PointF(0, 0)) {}

	void OnUpdate(UpdateEventArgs& e) override {

		if (Keyboard::KeyPressed(Key::F5))
			MyGame.Rooms().GotoRoom(MyGame.Rooms().CurrentRoom()->Id());

		if (Keyboard::KeyPressed(Key::O)) {
			std::cout << MyGame.Resources().Tilesets()[TILESET_1]->TileCount();
		}

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawBitmap(0, 0, &MyGame.Resources().Tilesets()[TILESET_1]->TileAt(5));

		CollisionManager* cm = (CollisionManager*)&MyGame.Collisions();
		CollisionGrid* cg = (CollisionGrid*)cm->BroadPhase();

		cg->DrawCells(e);

	}

};

class oBall : public Object {

public:
	oBall(float x, float y) :
		Object(1, PointF(x, y)) {

		_radius = Random::Float(10, 25);
		_velocity = Vector2d::FromDirection(Random::Float(0, 360), Random::Float(0.1, 1));

	}

	void OnCreate(CreateEventArgs& e) override {

		ICollisionBody* body = MyGame.Collisions().CreateBody(this);

		body->SetHitMask(CircleHitMask::Create(_radius));
		body->Filter().SetCategoryBits(0b010);
		body->Filter().SetMaskBits(0b010);

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(PointF(X() + 2, Y() + 2), _radius, Color::FromArgbf(0, 0, 0, 0.2), 2);
		e.Graphics().DrawCircle(PointF(X(), Y()), _radius, Color::LtGrey, 2);

	}
	void OnUpdate(UpdateEventArgs& e) override {

		Object::OnUpdate(e);

		float r = _radius;
		IRoom* room = MyGame.Rooms().CurrentRoom();

		if ((Y() - r < 0 && _velocity.Y() < 0) || (Y() + r > room->Height() && _velocity.Y() > 0))
			_velocity.SetY(-_velocity.Y());

		if ((X() - r < 0 && _velocity.X() < 0) || (X() + r > room->Width() && _velocity.X() > 0))
			_velocity.SetX(-_velocity.X());

		SetPosition(Position() + _velocity);

		// Slowdown.
		_velocity *= 0.99f;

	}
	void OnCollision(CollisionEventArgs& e) override {

		MyGame.Collisions().MoveOutsideBody(_body, e.Body(),
			PointDirection(e.Object()->Position(), Position()),
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
	ICollisionBody* _body;

};

class oMouseBox : public Object {

public:
	oMouseBox() :
		Object(0, Mouse::Position()) {

		_last_position = Position();

	}

	void OnCreate(CreateEventArgs& e) override {

		ICollisionBody* body = MyGame.Collisions().CreateBody(this);

		body->SetHitMask(CircleHitMask::Create(10.0f));
		body->Filter().SetCategoryBits(0b001);
		body->Filter().SetMaskBits(0b010);

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawCircle(Position(), 10, Color::AliceBlue, 1);

	}
	void OnUpdate(UpdateEventArgs& e) override {

		_last_position = Position();

		SetPosition(Mouse::Position());

	}
	void OnCollision(CollisionEventArgs& e) override {

		switch (e.Object()->Id()) {

		case 1: {

			oBall* ptr = (oBall*)e.Object();

			Vector2d move_vec(_last_position, Position());
			Vector2d dir_vec = Vector2d(Position(), ptr->Position()) + move_vec;

			Vector2d tot_vec = Vector2d::FromDirection(move_vec.Direction(), dir_vec.Length());

			// Set the ball's new velocity.	
			ptr->SetVelocity(tot_vec);

			// Move the ball outside of the cursor.
			if (tot_vec.Length() > 0)
				MyGame.Collisions().MoveOutsideBody(e.Body(), _body, PointDirection(e.Object()->Position(), Position()), 1);

			break;

		}
		}

	}

private:
	PointF _last_position;
	ICollisionBody* _body;

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

	void OnSetUp() override {

		SetBackgroundColor(Color::Silver);

		Backgrounds()->BackgroundAdd(MyGame.Resources().Backgrounds(BACKGROUND_1));
		Backgrounds()->PropertiesAt(0).SetTiledHorizontally(true);
		Backgrounds()->PropertiesAt(0).SetVelocity(Vector2d::FromDirection(0, 1));

		Objects()->AddInstance(Object::Create<oController>());
		Objects()->AddInstance(Object::Create<oMouseBox>());

		for (int i = 0; i < 100; ++i)
			Objects()->AddInstance(Object::Create<oBall>(Random::Float(Width()), Random::Float(Height())));


	}
	void OnRoomEnter(RoomEnterEventArgs& e) override {

		std::cout << "Entering room " << this << " with " << Objects()->InstanceCount() << " instances\n";

	}
	void OnRoomExit(RoomExitEventArgs& e) override {

		std::cout << "Exiting room " << this << " with " << Objects()->InstanceCount() << " instances\n";

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

class FreeFpsBall : public Object {

public:
	FreeFpsBall() : Object(0, PointF(0, 300)) {
	
		_delay = 0;
		_elapsed.Start();

	}
	void OnUpdate(UpdateEventArgs& e) override {

		SetX(X() + 500 * e.Delta());
		//std::cout << e.Delta() << std::endl;
		if (X() > MyGame.Rooms().CurrentRoom()->Width()) {
			SetX(0);
			std::cout << "TIME ELAPSED: " << _elapsed.SecondsElapsed() << std::endl;
			_elapsed.Reset();
		}

		if (Keyboard::KeyPressed(Key::KeyPadPlus))
			_delay += 100;
		if (Keyboard::KeyPressed(Key::KeyPadMinus))
			_delay -= 100;

		Threading::Thread::Sleep(_delay);

	}
	void OnDraw(DrawEventArgs& e) override {

		e.Graphics().DrawFilledCircle(Position(), 15, Color::Red);

	}

private:
	long _delay;
	Stopwatch _elapsed;

};

class TestRoom2 : public MyRoom {

public:
	TestRoom2() : MyRoom(960, 720) {}

private:
	void OnSetUp() override {
		Objects()->AddInstance(Object::Create<FreeFpsBall>());
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
	MyGame.Properties().Fps = FLT_MAX;
	MyGame.Properties().IsFixedFrameRate = false;
	
	// Load resources.
	MyGame.Resources().Tilesets().Add(TILESET_1, Resource::Create<Tileset>("data/test/tileset1.png", SizeI(32, 32)));
	MyGame.Resources().Backgrounds().Add(BACKGROUND_1, Resource::Create<Background>("data/test/tileset1.png"));
	MyGame.Resources().Sprites().Add(BACKGROUND_1, Sprite::Create("data/test/tileset1.png"));

	// Set up the first scene.
	SizeI large_size(MyGame.Properties().DisplaySize.Width() * 2, MyGame.Properties().DisplaySize.Height() * 2);
	SizeI medium_size(1000, 800);
	SizeI small_size(640, 480);
	MyGame.Rooms().AddRoom(Room::Create<TestRoom2>());
	MyGame.Rooms().AddRoom(Room::Create<editor::LevelEditor>(large_size, MyGame.Properties().DisplaySize));
	//MyGame.Rooms().AddRoom(Room::Create<TestRoom>());
	MyGame.Rooms().SetRoomTransition(RoomTransitionFade::Create(Color::Black, true));

	// Run the main game loop.
	MyGame.Loop();

	return 0;

}