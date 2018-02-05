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
			_manager->RemoveBody(this);

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