#pragma once
#include "CollisionFilter.h"
#include "IHitMask.h"

namespace hvn3 {

	class Collider {

	public:
		Collider(Positionable2dBase<float>* tracking_object);

		RectangleF AABB() const;

		IHitMask* HitMask();
		const IHitMask* HitMask() const;
		void SetHitMask(IHitMask* mask);
		//void SetHitMask(const RectangleF& mask);
		//void SetHitMask(const CircleF& mask);
		//void SetHitMask(const LineF& mask);

		CollisionFilter& Filter();
		const CollisionFilter& Filter() const;

		Positionable2dBase<float>* TrackingObject();
		const Positionable2dBase<float>* TrackingObject() const;
		
	private:
		Positionable2dBase<float>* _tracking_object;
		IHitMask* _mask;
		CollisionFilter _filter;

	};

}