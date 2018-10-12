#pragma once
#include "hvn3/math/Rectangle.h"
#include "hvn3/math/Circle.h"
#include "hvn3/math/Line.h"
#include <memory>

namespace hvn3 {

	struct CollisionResult;

	class HitMask {

		enum HITMASK_TYPE {
			HITMASK_TYPE_NULL,
			HITMASK_TYPE_RECTANGLE,
			HITMASK_TYPE_CIRCLE,
			HITMASK_TYPE_LINE
		};

		union mask_data_type  {

			RectangleF rectangle;
			CircleF circle;
			LineF line;

			mask_data_type() {}

		};

	public:
		HitMask();
		HitMask(const RectangleF& mask);
		HitMask(const CircleF& mask);
		HitMask(const LineF& mask);
		HitMask(const HitMask& other);

		void SetOffset(const PointF& offset);
		PointF Offset() const;

		RectangleF AABB() const;

		bool TestCollision(const HitMask& other, CollisionResult& manifold) const;
		bool TestCollision(const RectangleF& other, CollisionResult& manifold) const;
		bool TestCollision(const CircleF& other, CollisionResult& manifold) const;
		bool TestCollision(const LineF& other, CollisionResult& manifold) const;
		bool TestCollisionAt(const PointF& at, const HitMask& other, CollisionResult& manifold);

		HitMask& operator=(const HitMask& other);
		explicit operator bool() const;

	private:
		mask_data_type _mask;
		HITMASK_TYPE _type;

		void _copyAssign(const HitMask& other);

	};

}