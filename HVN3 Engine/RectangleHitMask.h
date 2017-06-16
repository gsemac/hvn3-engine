#pragma once
#include "IHitMask.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

	class RectangleHitMask : public IHitMask, public UniqueCreateableBase<IHitMask, RectangleHitMask> {

	public:
		RectangleHitMask(const RectangleF& bounds);

		void SetOffset(const PointF& offet) override;
		PointF Offset() const override;

		RectangleF AABB() const override;

		bool TestIntersection(const IHitMask* other) const override;
		bool TestIntersection(const RectangleF& other) const override;
		bool TestIntersection(const CircleF& other) const override;
		bool TestIntersection(const LineF& other) const override;

	private:
		RectangleF _mask;

	};

}