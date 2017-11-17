#pragma once
#include "ICollisionBody.h"

namespace hvn3 {

	class CollisionBodyBase : public ICollisionBody {

	public:
		CollisionBodyBase();

		PointF Position() const override;
		void SetPosition(const PointF& position) override;
		void SetPosition(float x, float y) override;
		RectangleF AABB() const override;
		CollisionFilter& Filter() override;
		const CollisionFilter& Filter() const override;
		HitMaskPtr& HitMask() override;
		const HitMaskPtr& HitMask() const override;
		void SetHitMask(HitMaskPtr& mask) override;
		bool IsSolid() const override;
		void SetSolid(bool value) override;

		void Destroy() override;
		bool IsDestroyed() const override;
		void OnDestroy(DestroyEventArgs& e) override;

		bool PlaceFree(const PointF& position) override;
		bool PlaceFreeIf(const PointF& position, const std::function<bool(ICollisionBody*)>& condition) override;
		bool MoveContact(float direction, float max_distance) override;
		bool MoveContactIf(float direction, float max_distance, const std::function<bool(ICollisionBody*)>& condition) override;
		bool MoveOutside(float direction, float max_distance) override;
		bool MoveOutsideBody(ICollisionBody* other, float direction, float max_distance) override;

	protected:
		IBroadPhase* BroadPhase();
		INarrowPhase* NarrowPhase();

	private:
		IBroadPhase* _broad_phase;		// The broad phase manager that manages this body.
		INarrowPhase* _narrow_phase;	// The narrow phase manager that manages this body.
		bool _is_solid;					// If true, the body is solid.
		bool _is_destroyed;				// If true, the body has been destroyed.
		HitMaskPtr _mask;
		CollisionFilter _filter;

		bool _phasePairSet() const;
		void _setCollisionPhasePair(IBroadPhase* broad_phase, INarrowPhase* narrow_phase) override;

	};

}