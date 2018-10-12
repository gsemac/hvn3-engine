#include "hvn3/collision/CollisionUtils.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/math/GeometryUtils.h"	

#include <cassert>

namespace hvn3 {

	HitMask::HitMask() {

		_type = HITMASK_TYPE_NULL;

	}
	HitMask::HitMask(const RectangleF& mask) {

		_type = HITMASK_TYPE_RECTANGLE;
		new(&_mask.rectangle) RectangleF(mask);

	}
	HitMask::HitMask(const CircleF& mask) {

		_type = HITMASK_TYPE_CIRCLE;
		new(&_mask.circle) CircleF(mask);

	}
	HitMask::HitMask(const LineF& mask) {

		_type = HITMASK_TYPE_LINE;
		new(&_mask.line) LineF(mask);

	}
	HitMask::HitMask::HitMask(const HitMask& other) {
		_copyAssign(other);
	}
	void HitMask::SetOffset(const PointF& offset) {

		assert(_type != HITMASK_TYPE_NULL);

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			_mask.rectangle.SetPosition(offset);
			break;
		case HITMASK_TYPE_CIRCLE:
			_mask.circle.SetPosition(offset);
			break;
		case HITMASK_TYPE_LINE:
			_mask.line = LineF(offset, (_mask.line.Second() - _mask.line.First()) + offset);
			break;
		}

	}
	PointF HitMask::Offset() const {

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			return _mask.rectangle.Position();
			break;
		case HITMASK_TYPE_CIRCLE:
			return _mask.circle.Position();
			break;
		case HITMASK_TYPE_LINE:
			return _mask.line.First();
			break;
		default:
			return PointF(0.0f, 0.0f);
			break;
		}

	}
	RectangleF HitMask::AABB() const {

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			return _mask.rectangle;
			break;
		case HITMASK_TYPE_CIRCLE:
			return RectangleF(
				PointF(_mask.circle.X() - _mask.circle.Radius(), _mask.circle.Y() - _mask.circle.Radius()),
				PointF(_mask.circle.X() + _mask.circle.Radius(), _mask.circle.Y() + _mask.circle.Radius())
			);
			break;
		case HITMASK_TYPE_LINE:
			return RectangleF(_mask.line.First(), _mask.line.Second());
			break;
		default:
			return RectangleF(0.0f, 0.0f, 0.0f, 0.0f);
			break;
		}

	}
	bool HitMask::TestCollision(const HitMask& other, CollisionResult& manifold) const {

		if (_type == HITMASK_TYPE_NULL)
			return false;

		switch (other._type) {
		case HITMASK_TYPE_RECTANGLE:
			return TestCollision(other._mask.rectangle, manifold);
			break;
		case HITMASK_TYPE_CIRCLE:
			return TestCollision(other._mask.circle, manifold);
			break;
		case HITMASK_TYPE_LINE:
			return TestCollision(other._mask.line, manifold);
			break;
		default:
			return false;
			break;
		}

	}
	bool HitMask::TestCollision(const RectangleF& other, CollisionResult& manifold) const {

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			return ResolveCollision(_mask.rectangle, other, manifold);
			break;
		case HITMASK_TYPE_CIRCLE:
			return Math::Geometry::TestIntersection(other, _mask.circle);
			break;
		case HITMASK_TYPE_LINE:
			return Math::Geometry::TestIntersection(other, _mask.line);
			break;
		default:
			throw System::NotImplementedException();
			break;
		}

	}
	bool HitMask::TestCollision(const CircleF& other, CollisionResult& manifold) const {

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			return Math::Geometry::TestIntersection(_mask.rectangle, other);
			break;
		case HITMASK_TYPE_CIRCLE:
			return Math::Geometry::TestIntersection(_mask.circle, other);
			break;
		case HITMASK_TYPE_LINE:
			return Math::Geometry::TestIntersection(other, _mask.line);
			break;
		default:
			throw System::NotImplementedException();
			break;
		}

	}
	bool HitMask::TestCollision(const LineF& other, CollisionResult& manifold) const {

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			return Math::Geometry::TestIntersection(_mask.rectangle, other);
			break;
		case HITMASK_TYPE_CIRCLE:
			return Math::Geometry::TestIntersection(_mask.circle, other);
			break;
		case HITMASK_TYPE_LINE:
			return Math::Geometry::TestIntersection(_mask.line, other);
			break;
		default:
			throw System::NotImplementedException();
			break;
		}

	}
	bool HitMask::TestCollisionAt(const PointF& at, const HitMask& other, CollisionResult& manifold) {

		PointF offset = Offset();
		
		SetOffset(offset + at);

		bool result = TestCollision(other, manifold);

		SetOffset(offset);

		return result;

	}

	HitMask& HitMask::operator=(const HitMask& other) {
		
		_copyAssign(other);

		return *this;

	}
	HitMask::operator bool() const {
		return _type != HITMASK_TYPE_NULL;
	}

	void HitMask::_copyAssign(const HitMask& other) {

		_type = other._type;

		switch (_type) {
		case HITMASK_TYPE_RECTANGLE:
			new(&_mask.rectangle) RectangleF(other._mask.rectangle);
			break;
		case HITMASK_TYPE_CIRCLE:
			new(&_mask.circle) CircleF(other._mask.circle);
			break;
		case HITMASK_TYPE_LINE:
			new(&_mask.line) LineF(other._mask.line);
			break;
		}

	}

}