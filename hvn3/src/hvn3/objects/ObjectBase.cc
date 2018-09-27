#include "hvn3/objects/ObjectBase.h"
#define DEFAULT_OBJECTBASE_FLAGS static_cast<hvn3::ObjectFlags>(0)

namespace hvn3 {

	ObjectBase::ObjectBase(ObjectId id) :
		ObjectBase(id, 0.0f, 0.0f) {
	}
	ObjectBase::ObjectBase(ObjectId id, const PointF& position) :
		ObjectBase(id, position.X(), position.Y()) {
	}
	ObjectBase::ObjectBase(ObjectId id, float x, float y) :
		ObjectBase(id, 0.0f, 0.0f, DEFAULT_OBJECTBASE_FLAGS) {
	}
	ObjectBase::ObjectBase(ObjectId id, ObjectFlags flags) :
		ObjectBase(id, 0.0f, 0.0f, flags) {
	}
	ObjectBase::ObjectBase(ObjectId id, const PointF& position, ObjectFlags flags) :
		ObjectBase(id, position.X(), position.Y(), flags) {
	}
	ObjectBase::ObjectBase(ObjectId id, float x, float y, ObjectFlags flags) :
		_position(x, y) {

		_id = id;
		_flags = flags;
		_depth = 0;
		_is_destroyed = false;
		_is_active = true;

	}

	void ObjectBase::OnCreate(CreateEventArgs& e) {}
	void ObjectBase::OnDestroy(DestroyEventArgs& e) {}
	void ObjectBase::OnContextChanged(ContextChangedEventArgs& e) {}

	ObjectId ObjectBase::Id() const {

		return _id;

	}
	int ObjectBase::Depth() const {

		return _depth;

	}
	void ObjectBase::SetDepth(int depth) {

		_depth = depth;

	}
	bool ObjectBase::IsDestroyed() const {

		return _is_destroyed;

	}
	void ObjectBase::Destroy() {

		_is_destroyed = true;

	}
	bool ObjectBase::IsActive() const {

		return _is_active;

	}
	void ObjectBase::SetActive(bool value) {

		_is_active = value;

	}
	ObjectFlags ObjectBase::Flags() const {

		return _flags;

	}
	void ObjectBase::SetFlags(ObjectFlags flags) {

		_flags = flags;

	}

	void ObjectBase::OnDraw(DrawEventArgs& e) {
	}
	void ObjectBase::OnUpdate(UpdateEventArgs& e) {
	}

	void ObjectBase::OnCollide(CollisionEventArgs& e) {
	}

	float ObjectBase::X() const {

		return _position.X();

	}
	float ObjectBase::Y() const {

		return _position.Y();

	}
	void ObjectBase::SetX(float x) {
		SetPosition(PointF(x, Y()));
	}
	void ObjectBase::SetY(float y) {
		SetPosition(PointF(X(), y));
	}
	PointF ObjectBase::Position() const {
		return _position;
	}
	void ObjectBase::SetPosition(const PointF& position) {
		_position = position;
	}
	void ObjectBase::SetPosition(float x, float y) {
		SetPosition(PointF(x, y));
	}

}