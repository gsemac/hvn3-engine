#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/objects/Object.h"
#include "hvn3/physics/IPhysicsBody.h"
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

		if (!HasFlag(flags, ObjectFlags::DisableCollisions)) {
			// Create collision body
			if (HasFlag(flags, ObjectFlags::EnablePhysics)) {
				// Create physics body
			}
		}

	}

	void Object::OnCreate(CreateEventArgs& e) {

		//if (CollisionBody())
		//	e.Collisions().AddBody(CollisionBody());

		//if (PhysicsBody())
		//	e.Physics().AddBody(PhysicsBody());

	}
	void Object::OnUpdate(UpdateEventArgs& e) {

		if (!HasFlag(Flags(), ObjectFlags::EnablePhysics))
			if (HasFlag(Flags(), ObjectFlags::Solid) && _collision_body)
				e.Collisions().MoveContact(_collision_body, Velocity().Direction(), Velocity().Length());
			else
				SetPosition(Position() + Velocity() * e.Delta());

		_renderer.UpdateAnimation(e.Delta());

	}
	void Object::OnDraw(DrawEventArgs& e) {

		_renderer.DrawSprite(e.Graphics(), Sprite(), Position());

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
	const Graphics::SpriteRenderer& Object::Renderer() const {

		return _renderer;

	}
	const Vector2d& Object::Velocity() const {

		if (_physics_body)
			return _physics_body->LinearVelocity();
		else
			return _velocity;

	}
	void Object::SetVelocity(const Vector2d& other) {

		if (_physics_body)
			_physics_body->SetLinearVelocity(other);
		else
			_velocity = other;

	}
	void Object::SetVelocity(float x, float y) {

		SetVelocity(Vector2d(x, y));

	}
	void Object::AddVelocity(const Vector2d& other) {

		if (_physics_body)
			_physics_body->SetLinearVelocity(Vector2d(_physics_body->LinearVelocity().X() + other.X(), _physics_body->LinearVelocity().Y() + other.Y()));
		else
			_velocity = Vector2d(_velocity.X() + other.X(), _velocity.Y() + other.Y());

	}
	void Object::AddVelocity(float x, float y) {

		AddVelocity(Vector2d(x, y));

	}
	CollisionBodyPtr& Object::GetCollisionBody() {

		return _collision_body;

	}
	const CollisionBodyPtr& Object::CollisionBody() const {

		return _collision_body;

	}
	PhysicsBodyPtr& Object::GetPhysicsBody() {

		return _physics_body;

	}
	const PhysicsBodyPtr& Object::PhysicsBody() const {

		return _physics_body;

	}

}