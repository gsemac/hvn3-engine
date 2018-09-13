#pragma once
#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/collision/ICollider.h"

namespace hvn3 {

	class ColliderBase :
		public ICollider {

	public:
		ColliderBase();

		PointF Position() const override;
		void SetPosition(const PointF& position) override;
		void SetPosition(float x, float y) override;
		RectangleF AABB() const override;
		CategoryFilter& GetCategory() override;
		const CategoryFilter& Category() const override;
		hvn3::HitMask& GetHitMask() override;
		const hvn3::HitMask& HitMask() const override;
		void SetHitMask(const hvn3::HitMask& mask) override;

		ColliderFlags Flags() const override;
		void SetFlags(ColliderFlags flags) override;

		void Destroy() override;
		bool IsDestroyed() const override;
		void OnDestroy(DestroyEventArgs& e) override;

	private:
		hvn3::HitMask _mask;
		CategoryFilter _filter;
		ColliderFlags _flags;
		bool _is_destroyed;

	};

}