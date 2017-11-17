#include "collision/CollisionBodyBase.h"
#include "collision/IBroadPhase.h"
#include "collision/INarrowPhase.h"
#include "collision/CollisionManifold.h"
#include "collision/ICollisionManager.h"
#include "math/GeometryUtils.h"

namespace hvn3 {

	CollisionBodyBase::CollisionBodyBase() {

		_manager = nullptr;

		_is_solid = false;
		_is_destroyed = false;

	}
	CollisionBodyBase::~CollisionBodyBase() {

		if (_manager != nullptr)
			_manager->RemoveBody(*this);

	}

	PointF CollisionBodyBase::Position() const {

		return PointF(X(), Y());

	}
	void CollisionBodyBase::SetPosition(const PointF& position) {

		SetPosition(position.X(), position.Y());

	}
	void CollisionBodyBase::SetPosition(float x, float y) {

		SetX(x);
		SetY(y);

	}
	RectangleF CollisionBodyBase::AABB() const {

		// A rectangle of area 0 is returned if the body doesn't have a hitmask.
		if (!HitMask())
			return RectangleF(X(), Y(), 0, 0);

		RectangleF aabb = HitMask()->AABB();
		aabb.Translate(X(), Y());

		return aabb;

	}
	CollisionFilter& CollisionBodyBase::Filter() {

		return _filter;

	}
	const CollisionFilter& CollisionBodyBase::Filter() const {

		return _filter;

	}
	HitMaskPtr& CollisionBodyBase::HitMask() {

		return _mask;

	}
	const HitMaskPtr& CollisionBodyBase::HitMask() const {

		return _mask;

	}
	void CollisionBodyBase::SetHitMask(HitMaskPtr& mask) {

		_mask = std::move(mask);

	}
	bool CollisionBodyBase::IsSolid() const {

		return _is_solid;

	}
	void CollisionBodyBase::SetSolid(bool value) {

		_is_solid = value;

	}

	void CollisionBodyBase::Destroy() {

		_is_destroyed = true;

	}
	bool CollisionBodyBase::IsDestroyed() const {

		return _is_destroyed;

	}
	void CollisionBodyBase::OnDestroy(DestroyEventArgs& e) {}

	bool CollisionBodyBase::PlaceFree(const PointF& position) {

		return PlaceFreeIf(position, [](ICollisionBody*) { return true; });

	}
	bool CollisionBodyBase::PlaceFreeIf(const PointF& position, const std::function<bool(ICollisionBody*)>& condition) {

		if (!_managerSet())
			return true;

		// If the object does not have a collision mask, return true immediately (no collisions are possible).
		if (HitMask() == nullptr)
			return true;

		// Create a vector to store the results.
		IBroadPhase::collider_vector_type hits;

		// Get a list of all colliders that could potentially collide with the collider.
		_manager->BroadPhase().QueryRegion(AABB(), hits, Filter().MaskBits());

		// If the list is empty, the place is free.
		if (hits.size() == 0)
			return true;

		for (size_t i = 0; i < hits.size(); ++i) {

			// Ignore self and objects that don't meet the given condition.
			if (hits[i] == this || !condition(hits[i]))
				continue;

			// Check for a collision.
			CollisionManifold m;
			if (_manager->NarrowPhase().TestCollision(this, position, hits[i], hits[i]->Position(), m))
				return false;

		}

		// The collider did not collide with any other colliders, so the plaec is free.
		return true;

	}
	bool CollisionBodyBase::MoveContact(float direction, float max_distance) {

		return MoveContactIf(direction, max_distance, [](ICollisionBody*) { return true; });

	}
	bool CollisionBodyBase::MoveContactIf(float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) {

		float distance = 0.0f;
		float distance_per_step = Math::Min(1.0f, max_distance);
		PointF new_position = Math::Geometry::PointInDirection(Position(), direction, distance_per_step);
		bool place_free;

		// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
		while ((place_free = PlaceFreeIf(new_position, condition), place_free) && distance < (std::abs)(max_distance)) {

			SetPosition(new_position);

			new_position = Math::Geometry::PointInDirection(new_position, direction, distance_per_step);

			distance += distance_per_step;

		}

		return !place_free;

	}
	bool CollisionBodyBase::MoveOutside(float direction, float max_distance) {

		PointF pos = Position();
		float dist = 0.0f;
		float distance_per_step = Math::Min(1.0f, max_distance);
		bool place_free;

		// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
		while ((place_free = PlaceFree(pos), !place_free) && dist < max_distance) {

			pos = Math::Geometry::PointInDirection(pos, direction, distance_per_step);

			dist += distance_per_step;

		}

		SetPosition(pos);

		return place_free;

	}
	bool CollisionBodyBase::MoveOutsideBody(ICollisionBody* other, float direction, float max_distance) {

		if (!_managerSet())
			return true;

		float dist = 0.0f;
		float distance_per_step = Math::Min(1.0f, max_distance);
		bool place_free;
		CollisionManifold m;

		// It's important that we check if the place is free before doing the distance check (what if the user passes in a distance of 0?).
		while ((place_free = _manager->NarrowPhase().TestCollision(this, other, m), place_free) && dist < (std::abs)(max_distance)) {

			SetPosition(Math::Geometry::PointInDirection(Position(), direction, distance_per_step));

			dist += distance_per_step;

		}

		return place_free;

	}



	ICollisionManager* CollisionBodyBase::Manager() {

		return _manager;

	}



	bool CollisionBodyBase::_managerSet() const {

		return (_manager != nullptr);

	}
	void CollisionBodyBase::_setManager(ICollisionManager* manager) {

		_manager = manager;

	}

}