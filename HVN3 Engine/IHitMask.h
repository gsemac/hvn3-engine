#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include <memory>

namespace hvn3 {

	class IHitMask;
	struct CollisionManifold;

	typedef std::unique_ptr<IHitMask> HitMaskPtr;

	class IHitMask {

	public:
		virtual void SetOffset(const PointF& offet) = 0;
		virtual PointF Offset() const = 0;

		virtual RectangleF AABB() const = 0;

		virtual bool TestCollision(const HitMaskPtr& other, CollisionManifold& manifold) const = 0;
		virtual bool TestCollision(const RectangleF& other, CollisionManifold& manifold) const = 0;
		virtual bool TestCollision(const CircleF& other, CollisionManifold& manifold) const = 0;
		virtual bool TestCollision(const LineF& other, CollisionManifold& manifold) const = 0;

	};

}