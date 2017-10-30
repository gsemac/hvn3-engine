#pragma once
#include "test/Global.h"
#include "physics/BasicPhysicsManager.h"

hvn3::Physics::BasicPhysicsManager PhysicsManager(nullptr);

class DynamicBox : 
	public hvn3::Object {

public:
	DynamicBox(float x, float y) :
		Object(1, hvn3::PointF(x, y)) {}

	void OnCreate(hvn3::CreateEventArgs& e) override {

		_collision_body = GameState.Collisions()->CreateBody(this);
		_collision_body->SetHitMask(hvn3::Collision::HitMaskPtr(new hvn3::Collision::RectangleHitMask(hvn3::RectangleF(32.0f, 32.0f))));
		_collision_body->Filter().SetCategoryBits(1);
		_collision_body->Filter().SetMaskBits(1 | 2);
		
		_physics_body = PhysicsManager.CreateBody(_collision_body);
		_physics_body->SetType(hvn3::Physics::BodyType::Dynamic);

		if (hvn3::Random::Boolean())
			_physics_body->SetLinearVelocity(hvn3::Vector2d::FromDirection(hvn3::DIRECTION_RIGHT, 40.0f));

	}
	void OnDestroy(hvn3::DestroyEventArgs& e) override {
		// Destroy bodies
	}
	void OnDraw(hvn3::DrawEventArgs& e) override {
		
		e.Graphics().DrawFilledRectangle(X(), Y(), 32.0f, 32.0f, hvn3::Color::White);
		e.Graphics().DrawRectangle(X(), Y(), 32.0f, 32.0f, hvn3::Color::Black, 2.0f);

	}
	void OnCollision(hvn3::CollisionEventArgs& e) override {

	}
	void OnUpdate(hvn3::UpdateEventArgs& e) override {
	
		//std::cout << _physics_body->LinearVelocity().Direction() << std::endl;
	
	}

private:
	hvn3::Physics::IPhysicsBody* _physics_body;
	hvn3::Collision::ICollisionBody* _collision_body;

};

class StaticBox : public hvn3::Object {
public:
	StaticBox(float x, float y) : hvn3::Object(2, hvn3::PointF(x, y)) {}
	void OnCreate(hvn3::CreateEventArgs& e) override {
		_collision_body = GameState.Collisions()->CreateBody(this);
		_collision_body->SetHitMask(hvn3::Collision::HitMaskPtr(new hvn3::Collision::RectangleHitMask(hvn3::RectangleF(512.0f, 16.0f))));
		_collision_body->Filter().SetCategoryBits(2);
		_physics_body = PhysicsManager.CreateBody(_collision_body);
		_physics_body->SetType(hvn3::Physics::BodyType::Static);
		_physics_body->SetMaterial(hvn3::Physics::Material::Static);
	}
	void OnDestroy(hvn3::DestroyEventArgs& e) override {
		// Destroy bodies
	}
	void OnDraw(hvn3::DrawEventArgs& e) override {
		e.Graphics().DrawFilledRectangle(X(), Y(), 512.0f, 16.0f, hvn3::Color::DimGrey);
	}
private:
	hvn3::Physics::IPhysicsBody* _physics_body;
	hvn3::Collision::ICollisionBody* _collision_body;
};

class PhysicsRoom : 
	public hvn3::Room {

public:
	PhysicsRoom() : 
		hvn3::Room(1, hvn3::SizeI(640, 480)) {}

	void OnUpdate(hvn3::UpdateEventArgs& e) override {

		Room::OnUpdate(e);
		PhysicsManager.OnUpdate(e);
		
		if (hvn3::Keyboard::KeyPressed(hvn3::Key::F5))
			GameState.Rooms()->RestartRoom();
	
		if (hvn3::Mouse::ButtonPressed(hvn3::MouseButton::Left))
			Objects()->AddInstance(new DynamicBox(hvn3::Mouse::X, hvn3::Mouse::Y));
	
	}

protected:
	void OnSetUp() override {

		PhysicsManager = hvn3::Physics::BasicPhysicsManager(Collisions());
		
		SetBackgroundColor(hvn3::Color::Silver);

		Objects()->AddInstance(new StaticBox(64.0f, 400.0f));
	
	}

};