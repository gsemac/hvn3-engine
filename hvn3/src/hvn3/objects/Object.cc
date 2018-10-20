#include "hvn3/collision/Collider.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/CoreDefs.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/objects/Object.h"
#include "hvn3/physics/IPhysicsBody.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/sprites/Sprite.h"

#define DEFAULT_OBJECTBASE_FLAGS static_cast<hvn3::ObjectFlags>(0)

namespace hvn3 {

	Object::Object() :
		Object(NoOne) {
	}
	Object::Object(float x, float y) :
		Object(NoOne, x, y) {
	}
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

		_sprite_id = Preview::NULL_ASSET_ID;

	}
	Object::~Object() {}

	void Object::OnCreate(CreateEventArgs& e) {

		_context = e.Context();

		if (!HasFlag(Flags(), ObjectFlags::NoCollisions)) {

			_collider = hvn3::make_collider<>(this);

			if (_context.Local().IsRegistered<hvn3::COLLISION_MANAGER>())
				_context.Get<hvn3::COLLISION_MANAGER>().Add(_collider);

		}

	}
	void Object::OnUpdate(UpdateEventArgs& e) {

		// We will only manually adjust the object's velocity if it isn't governed by the physics manager.
		//if (!HasFlag(Flags(), ObjectFlags::EnablePhysics) || _physics_body == nullptr) {
		if (HasFlag(Flags(), ObjectFlags::Solid) && _collider && _context.IsRegistered<hvn3::COLLISION_MANAGER>())
			// The object is solid, so don't let it intersect any objects it can collide with.
			_context.Get<hvn3::COLLISION_MANAGER>().MoveContact(_collider.get(), Velocity().Direction(), Velocity().Length());
		else
			// The object is not solid, so simply move it to its next position.
			SetPosition(Position() + Velocity() * static_cast<float>(e.Delta()));
		//}

		_sprite_renderer.UpdateAnimation(e.Delta());

	}
	void Object::OnDraw(DrawEventArgs& e) {

		_sprite_renderer.DrawSprite(e.Graphics(), Position(), Sprite());

	}
	void Object::OnDestroy(DestroyEventArgs& e) {}
	void Object::OnContextChanged(ContextChangedEventArgs& e) {

		_context = e.Context();

	}

	const Vector2d& Object::Velocity() const {

		//if (_physics_body != nullptr)
		//	return _physics_body->LinearVelocity();
		//else
		return _velocity;

	}
	void Object::SetVelocity(const Vector2d& other) {

		/*	if (_physics_body != nullptr)
				_physics_body->SetLinearVelocity(other);
			else*/
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
	const class Sprite& Object::Sprite() const {

		if (_sprite_id != Preview::NULL_ASSET_ID && _context.IsRegistered<hvn3::ASSET_MANAGER>())
			return _context.Get<hvn3::ASSET_MANAGER>().Sprites(_sprite_id);

		return _sprite_renderer.Sprite();

	}
	void Object::SetSprite(const class Sprite& value) {

		_sprite_id = Preview::NULL_ASSET_ID;

		_sprite_renderer.SetSprite(value);

	}
	void Object::SetSprite(Preview::AssetId value) {

		_sprite_id = value;

	}
	SpriteRenderer& Object::Renderer() {
		return _sprite_renderer;
	}
	ColliderPtr& Object::Collider() {
		return _collider;
	}

}