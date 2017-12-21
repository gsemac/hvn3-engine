#include "hvn3/collision/CollisionBodyBase.h"
#include "hvn3/collision/IBroadPhase.h"
#include "hvn3/collision/INarrowPhase.h"
#include "hvn3/collision/CollisionManifold.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/math/GeometryUtils.h"

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
	CategoryFilter& CollisionBodyBase::Category() {

		return _filter;

	}
	const CategoryFilter& CollisionBodyBase::Category() const {

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
	bool CollisionBodyBase::PlaceFree(float x, float y) {

		return PlaceFree(PointF(x, y));

	}
	bool CollisionBodyBase::PlaceFreeIf(const PointF& position, const std::function<bool(ICollisionBody*)>& condition) {

		CollisionManifold m;

		return PlaceFreeIf(position, condition, m);

	}
	bool CollisionBodyBase::PlaceFreeIf(const PointF& position, const std::function<bool(ICollisionBody*)>& condition, CollisionManifold& manifold) {

		if (!_managerIsSet())
			return true;

		// If the object does not have a collision mask, return true immediately (no collisions are possible).
		if (HitMask() == nullptr)
			return true;

		// Create a vector to store the results.
		IBroadPhase::collider_vector_type hits;

		// Get a list of all colliders that could potentially collide with the collider.
		RectangleF aabb = AABB();
		aabb.Translate(-X(), -Y());
		aabb.Translate(position.X(), position.Y());
		_manager->BroadPhase().QueryRegion(aabb, hits, Category().MaskBits());

		// If the list is empty, the place is free.
		if (hits.size() == 0)
			return true;

		for (size_t i = 0; i < hits.size(); ++i) {

			// Ignore self and objects that don't meet the given condition.
			if (hits[i] == this || !condition(hits[i]))
				continue;

			// Check for a collision.
			if (_manager->NarrowPhase().TestCollision(this, position, hits[i], hits[i]->Position(), manifold))
				return false;

		}

		// The collider did not collide with any other colliders, so the place is free.
		return true;

	}
	bool CollisionBodyBase::MoveContact(float direction, float max_distance) {

		return MoveContactIf(direction, max_distance, [](ICollisionBody*) { return true; });

	}
	bool CollisionBodyBase::MoveContactIf(float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) {

		// If the distance is negative, reverse the direction and then make it positive.
		if (max_distance < 0.0f) {
			direction += 180.0f;
			max_distance = Math::Abs(max_distance);
		}

		// If the distance is 0, just return if the current position is free.
		if (Math::IsZero(max_distance))
			return PlaceFreeIf(Position(), condition);

		float distance = 0.0f;
		float distance_per_step = Math::Min(AABB().Width(), AABB().Height(), max_distance);
		PointF new_position = Position();
		bool place_free;

		if (distance_per_step <= 0.0f) {
			// The body doesn't have a valid AABB, and thus will never collide with anything. Just move to the position and return false to indicate no collisions occurred.
			SetPosition(Math::Geometry::PointInDirection(Position(), direction, max_distance));
			return false;
		}

		while (distance < (std::abs)(max_distance)) {
			PointF tentative_position = Math::Geometry::PointInDirection(new_position, direction, distance_per_step);
			place_free = PlaceFreeIf(tentative_position, condition);

			if (!place_free) {
				if (distance_per_step < 0.2f)
					break;
				else {
					distance_per_step /= 2.0f;
					continue;
				}
			}

			new_position = tentative_position;
			distance += distance_per_step;

		}

		SetPosition(new_position);

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

		if (!_managerIsSet())
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



	bool CollisionBodyBase::_managerIsSet() const {

		return (_manager != nullptr);

	}
	void CollisionBodyBase::_setManager(ICollisionManager* manager) {

		_manager = manager;

	}

}