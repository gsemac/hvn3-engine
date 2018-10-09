#include "hvn3/collision/ICollider.h"
#include "hvn3/collision/ICollisionManager.h"
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
	}
	Object::~Object() {}

	void Object::OnCreate(CreateEventArgs& e) {}
	void Object::OnUpdate(UpdateEventArgs& e) {
		//// We will only manually adjust the object's velocity if it isn't governed by the physics manager.
		//if (!HasFlag(Flags(), ObjectFlags::EnablePhysics) || _physics_body == nullptr) {
		//	if (HasFlag(Flags(), ObjectFlags::Solid) && _collision_body != nullptr)
		//		// The object is solid, so don't let it intersect any objects it can collide with.
		//		Context().Collisions().MoveContact(_collision_body, Velocity().Direction(), Velocity().Length());
		//	else
		//		// The object is not solid, so simply move it to its next position.
		//		SetPosition(Position() + Velocity() * e.Delta());
		//}
		//_renderer.Step(e.Delta());
	}
	void Object::OnDraw(DrawEventArgs& e) {}
	void Object::OnDestroy(DestroyEventArgs& e) {}

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

}