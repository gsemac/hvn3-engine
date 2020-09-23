#include "hvn3/collision/CollisionUtils.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/math/GeometryUtils.h"	

#include <cassert>

namespace hvn3 {

	HitMask::HitMask() {

		_type = HITMASK_TYPE_NULL;

	}
	HitMask::HitMask(const rectangle_type& mask) {

		_type = HITMASK_TYPE_RECTANGLE;
		new(&_mask.rectangle) rectangle_type(mask);

	}
	HitMask::HitMask(const circle_type& mask) {

		_type = HITMASK_TYPE_CIRCLE;
		new(&_mask.circle) circle_type(mask);

	}
	HitMask::HitMask(const line_type& mask) {

		_type = HITMASK_TYPE_LINE;
		new(&_mask.line) line_type(mask);

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

		default:
			throw Exception("Invalid hitmask type.");

		}

	}
	PointF HitMask::Offset() const {

		switch (_type) {

		case HITMASK_TYPE_NULL:
			return PointF(0.0f, 0.0f);

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
			throw Exception("Invalid hitmask type.");

		}

	}

	RectangleF HitMask::Aabb() const {

		switch (_type) {

		case HITMASK_TYPE_NULL:
			return RectangleF(0.0f, 0.0f, 0.0f, 0.0f);

		case HITMASK_TYPE_RECTANGLE:
			return CalculateAabb(_mask.rectangle);

		case HITMASK_TYPE_CIRCLE:
			return CalculateAabb(_mask.circle);

		case HITMASK_TYPE_LINE:
			return CalculateAabb(_mask.line);

		default:
			throw Exception("Invalid hitmask type.");

		}

	}
	HitMask::HITMASK_TYPE HitMask::Type() const {
		return _type;
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

		case HITMASK_TYPE_NULL:
			break;

		case HITMASK_TYPE_RECTANGLE:

			new(&_mask.rectangle) rectangle_type(other._mask.rectangle);

			break;

		case HITMASK_TYPE_CIRCLE:

			new(&_mask.circle) circle_type(other._mask.circle);

			break;

		case HITMASK_TYPE_LINE:

			new(&_mask.line) line_type(other._mask.line);

			break;

		default:
			throw Exception("Invalid hitmask type.");

		}

	}

}