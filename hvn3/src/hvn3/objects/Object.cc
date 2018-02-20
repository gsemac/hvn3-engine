#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/objects/Object.h"
#include "hvn3/physics/IPhysicsBody.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/sprites/Sprite.h"
#define DEFAULT_OBJECTBASE_FLAGS static_cast<hvn3::ObjectFlags>(0)

namespace hvn3 {

	Object::Object(ObjectId id) :
		Object(id, 0.0f, 0.0f) {
	}
	Object::Object(ObjectId id, const PointF& position) :
		Object(id, position.X(), position.Y()) {
	}
	Object::Object(ObjectId id, float x, float y) :
		Object(id, x, y, DEFAULT_OBJECTBASE_FLAGS) {
	}
	Object::Object(ObjectId id, ObjectFlags flags) :
		Object(id, 0.0f, 0.0f, flags) {
	}
	Object::Object(ObjectId id, const PointF& position, ObjectFlags flags) :
		Object(id, position.X(), position.Y(), flags) {
	}
	Object::Object(ObjectId id, float x, float y, ObjectFlags flags) :
		ObjectBase(id, x, y, flags) {
		_collision_body = nullptr;
		_physics_body = nullptr;
	}
	Object::~Object() {
		_destroyCollisionBody();
		_destroyPhysicsBody();
	}

	void Object::OnCreate(CreateEventArgs& e) {
		if (!HasFlag(Flags(), ObjectFlags::NoCollisions)) {
			// Create a collision body for this object.
			_collision_body = e.Collisions().CreateBody(this);
			// Create a physics body for this object.
			if (HasFlag(Flags(), ObjectFlags::EnablePhysics) && _collision_body != nullptr) {
				_physics_body = e.Physics().CreateBody(_collision_body);
				// Apply the current velocity to the physics object (it may have been set before create event).
				_physics_body->SetLinearVelocity(_velocity);
			}
		}
	}
	void Object::OnUpdate(UpdateEventArgs& e) {
		// We will only manually adjust the object's velocity if it isn't governed by the physics manager.
		if (!HasFlag(Flags(), ObjectFlags::EnablePhysics) || _physics_body == nullptr) {
			if (HasFlag(Flags(), ObjectFlags::Solid) && _collision_body != nullptr)
				// The object is solid, so don't let it intersect any objects it can collide with.
				e.Collisions().MoveContact(_collision_body, Velocity().Direction(), Velocity().Length());
			else
				// The object is not solid, so simply move it to its next position.
				SetPosition(Position() + Velocity() * e.Delta());
		}
		_renderer.StepAnimation(e.Delta());
	}
	void Object::OnDraw(DrawEventArgs& e) {
		_renderer.DrawSprite(e.Graphics(), Sprite(), Position());
	}
	void Object::OnDestroy(DestroyEventArgs& e) {
		_destroyCollisionBody();
		_destroyPhysicsBody();
	}

	const Object::sprite_type& Object::Sprite() const {

		return _sprite;

	}
	Object::sprite_type& Object::GetSprite() {

		return _sprite;

	}
	void Object::SetSprite(const sprite_type& other) {

		_sprite = other;

	}
	void Object::SetSprite(sprite_type&& other) {

		_sprite = std::move(other);

	}
	const Graphics::SpriteRenderer& Object::Renderer() const {

		return _renderer;

	}
	Graphics::SpriteRenderer& Object::GetRenderer() {

		return _renderer;

	}
	void Object::SetRenderer(const Graphics::SpriteRenderer& other) {

		_renderer = other;

	}
	const Vector2d& Object::Velocity() const {
		if (_physics_body != nullptr)
			return _physics_body->LinearVelocity();
		else
			return _velocity;
	}
	void Object::SetVelocity(const Vector2d& other) {
		if (_physics_body != nullptr)
			_physics_body->SetLinearVelocity(other);
		else
			_velocity = other;
	}
	void Object::SetVelocity(float x, float y) {
		SetVelocity(Vector2d(x, y));
	}
	void Object::AddVelocity(const Vector2d& other) {
		SetVelocity(Velocity() + other);
	}
	void Object::AddVelocity(float x, float y) {
		AddVelocity(Vector2d(x, y));
	}
	ICollisionBody* Object::GetCollisionBody() {

		return _collision_body;

	}
	const ICollisionBody* Object::CollisionBody() const {

		return _collision_body;

	}
	Physics::IPhysicsBody* Object::GetPhysicsBody() {

		return _physics_body;

	}
	const Physics::IPhysicsBody* Object::PhysicsBody() const {

		return _physics_body;

	}



	void Object::_destroyCollisionBody() {
		if (_collision_body != nullptr)
			_collision_body->Destroy();
		_collision_body = nullptr;
	}
	void Object::_destroyPhysicsBody() {
		if (_physics_body != nullptr)
			_physics_body->Destroy();
		_physics_body = nullptr;
	}

}