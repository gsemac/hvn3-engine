#pragma once
#include "IHitMask.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

	class CircleHitMask : public IHitMask, public UniqueCreateableBase<IHitMask, CircleHitMask> {

	public:
		CircleHitMask(const CircleF& mask);

		void SetOffset(const PointF& offet) override;
		PointF Offset() const override;

		RectangleF AABB() const override;

		bool TestIntersection(const HitMaskPtr& other) const override;
		bool TestIntersection(const RectangleF& other) const override;
		bool TestIntersection(const CircleF& other) const override;
		bool TestIntersection(const LineF& other) const override;

	private:
		CircleF _mask;

	};

}