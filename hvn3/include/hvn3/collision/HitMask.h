#pragma once
#include "hvn3/math/Rectangle.h"
#include "hvn3/math/Circle.h"
#include "hvn3/math/Line.h"
#include <memory>

namespace hvn3 {

	struct CollisionResult;

	class HitMask {

	public:
		typedef RectangleF rectangle_type;
		typedef CircleF circle_type;
		typedef LineF line_type;

		enum HITMASK_TYPE {
			HITMASK_TYPE_NULL,
			HITMASK_TYPE_RECTANGLE,
			HITMASK_TYPE_CIRCLE,
			HITMASK_TYPE_LINE
		};

	private:
		union union_type {

			rectangle_type rectangle;
			circle_type circle;
			line_type line;

			union_type() {}

		};

	public:
		HitMask();
		HitMask(const rectangle_type& mask);
		HitMask(const circle_type& mask);
		HitMask(const line_type& mask);
		HitMask(const HitMask& other);

		void SetOffset(const PointF& offset);
		PointF Offset() const;

		RectangleF Aabb() const;
		HITMASK_TYPE Type() const;

		template<typename T> const T& GetBody(const PointF& offset = PointF(0.0f, 0.0f)) const {

			static_assert(false, "Invalid body type provided.");

			throw;

		}
		template<> const rectangle_type& GetBody(const PointF& offset) const {

			if (Type() == HITMASK_TYPE::HITMASK_TYPE_RECTANGLE)
				return _mask.rectangle;

			throw System::ArgumentException("HitMask type did not match.");

		}
		template<> const circle_type& GetBody(const PointF& offset) const {

			if (Type() == HITMASK_TYPE::HITMASK_TYPE_CIRCLE)
				return _mask.circle;

			throw System::ArgumentException("HitMask type did not match.");

		}
		template<> const line_type& GetBody(const PointF& offset) const {

			if (Type() == HITMASK_TYPE::HITMASK_TYPE_LINE)
				return _mask.line;

			throw System::ArgumentException("HitMask type did not match.");

		}

		HitMask& operator=(const HitMask& other);

		explicit operator bool() const;

	private:
		union_type _mask;
		HITMASK_TYPE _type;

		void _copyAssign(const HitMask& other);

	};

}