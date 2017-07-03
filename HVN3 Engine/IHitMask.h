#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"
#include <memory>

namespace hvn3 {

	class IHitMask;

	typedef std::unique_ptr<IHitMask> HitMaskPtr;

	class IHitMask {

	public:
		virtual void SetOffset(const PointF& offet) = 0;
		virtual PointF Offset() const = 0;

		virtual RectangleF AABB() const = 0;

		virtual bool TestIntersection(const HitMaskPtr& other) const = 0;
		virtual bool TestIntersection(const RectangleF& other) const = 0;
		virtual bool TestIntersection(const CircleF& other) const = 0;
		virtual bool TestIntersection(const LineF& other) const = 0;

	};

}