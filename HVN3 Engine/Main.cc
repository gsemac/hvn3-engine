#include "HVN3.h"
using namespace hvn3;

GameManager GameState;
Physics::BasicPhysicsManager PhysicsManager(nullptr);

class DynamicBox : public Object {
public:
	DynamicBox(float x, float y) : Object(1, PointF(x, y)) {}
	void OnCreate(CreateEventArgs& e) override {
		_collision_body = GameState.Collisions().CreateBody(this);
		_collision_body->SetHitMask(Collision::RectangleHitMask::Create(RectangleF(32.0f, 32.0f)));
		_collision_body->Filter().SetCategoryBits(1);
		_collision_body->Filter().SetMaskBits(1 | 2);
		_physics_body = PhysicsManager.CreateBody(_collision_body);
		_physics_body->SetType(Physics::BodyType::Dynamic);
		/*if (Random::Boolean())
			_physics_body->SetLinearVelocity(Vector2d::FromDirection(DIRECTION_RIGHT, 40.0f));*/
	}
	void OnDestroy(DestroyEventArgs& e) override {
		// Destroy bodies
	}
	void OnDraw(DrawEventArgs& e) override {
		e.Graphics().DrawFilledRectangle(X(), Y(), 32.0f, 32.0f, Color::White);
		e.Graphics().DrawRectangle(X(), Y(), 32.0f, 32.0f, Color::Black, 2.0f);
	}
	void OnCollision(CollisionEventArgs& e) override {
	}
	void OnUpdate(UpdateEventArgs& e) override {
		//std::cout << _physics_body->LinearVelocity().Direction() << std::endl;
	}
private:
	Physics::IPhysicsBody* _physics_body;
	Collision::ICollisionBody* _collision_body;
};

class StaticBox : public Object {
public:
	StaticBox(float x, float y) : Object(2, PointF(x, y)) {}
	void OnCreate(CreateEventArgs& e) override {
		_collision_body = GameState.Collisions().CreateBody(this);
		_collision_body->SetHitMask(Collision::RectangleHitMask::Create(RectangleF(512.0f, 16.0f)));
		_collision_body->Filter().SetCategoryBits(2);
		_physics_body = PhysicsManager.CreateBody(_collision_body);
		_physics_body->SetType(Physics::BodyType::Static);
		_physics_body->SetMaterial(Physics::Material::Static);
	}
	void OnDestroy(DestroyEventArgs& e) override {
		// Destroy bodies
	}
	void OnDraw(DrawEventArgs& e) override {
		e.Graphics().DrawFilledRectangle(X(), Y(), 512.0f, 16.0f, Color::DimGrey);
	}
private:
	Physics::IPhysicsBody* _physics_body;
	Collision::ICollisionBody* _collision_body;
};

class PhysicsRoom : public Room {
public:
	PhysicsRoom() : Room(1, SizeI(640, 480)) {}
	void OnUpdate(UpdateEventArgs& e) override {
		Room::OnUpdate(e);
		PhysicsManager.OnUpdate(e);
		if (Keyboard::KeyPressed(Key::F5))
			GameState.Rooms().RestartRoom();
		if(Mouse::ButtonPressed(MouseButton::Left))
			Objects()->AddInstance(Object::Create<DynamicBox>(Mouse::X, Mouse::Y));
	}
protected:
	void OnSetUp() override {
		PhysicsManager = Physics::BasicPhysicsManager(Collisions());
		SetBackgroundColor(Color::Silver);
		//Objects()->AddInstance(Object::Create<DynamicBox>(320.0f, 240.0f));
		//Objects()->AddInstance(Object::Create<DynamicBox>(280.0f, 240.0f));
		Objects()->AddInstance(Object::Create<StaticBox>(64.0f, 400.0f));
	}
};

int main(int argc, char *argv[]) {

	// Initialize game properties.
	GameState.Initialize(argc, argv);
	GameState.Properties().DebugMode = true;
	GameState.Properties().Fps = 60.0f;
	GameState.Properties().FixedFrameRate = true;

	// Set up the first room.
	GameState.Rooms().AddRoom(Room::Create<PhysicsRoom>());

	// Run the main game loop.
	GameState.Loop();

	return 0;

}