#pragma once
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/collision/CategoryFilter.h"

namespace hvn3 {

	class CollisionBodyBase : public ICollisionBody {

	public:
		CollisionBodyBase();

		PointF Position() const override;
		void SetPosition(const PointF& position) override;
		void SetPosition(float x, float y) override;
		RectangleF AABB() const override;
		CategoryFilter& GetCategory() override;
		const CategoryFilter& Category() const override;
		HitMaskPtr& GetHitMask() override;
		const HitMaskPtr& HitMask() const override;
		void SetHitMask(HitMaskPtr& mask) override;

		CollisionBodyFlags Flags() const override;
		void SetFlags(CollisionBodyFlags flags) override;

		void Destroy() override;
		bool IsDestroyed() const override;
		void OnDestroy(DestroyEventArgs& e) override;

	private:
		HitMaskPtr _mask;
		CategoryFilter _filter;
		CollisionBodyFlags _flags;
		bool _is_destroyed;

	};

}