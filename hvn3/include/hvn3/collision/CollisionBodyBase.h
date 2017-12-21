#pragma once
#include "hvn3/collision/ICollisionBody.h"

namespace hvn3 {

	class CollisionBodyBase : public ICollisionBody {

	public:
		CollisionBodyBase();
		~CollisionBodyBase() override;

		PointF Position() const override;
		void SetPosition(const PointF& position) override;
		void SetPosition(float x, float y) override;
		RectangleF AABB() const override;
		CategoryFilter& Category() override;
		const CategoryFilter& Category() const override;
		HitMaskPtr& HitMask() override;
		const HitMaskPtr& HitMask() const override;
		void SetHitMask(HitMaskPtr& mask) override;
		bool IsSolid() const override;
		void SetSolid(bool value) override;

		void Destroy() override;
		bool IsDestroyed() const override;
		void OnDestroy(DestroyEventArgs& e) override;

	protected:
		ICollisionManager* Manager();

	private:
		ICollisionManager* _manager;	// The collision manager that manages this body.
		bool _is_solid;					// If true, the body is solid.
		bool _is_destroyed;				// If true, the body has been destroyed.
		HitMaskPtr _mask;
		CategoryFilter _filter;

		bool _managerIsSet() const;
		void _setManager(ICollisionManager* manager) override;

	};

}