#pragma once
#include "ICollisionBody.h"

namespace hvn3 {

	class Object;

	class ObjectCollisionBody final : public ICollisionBody {

	public:
		ObjectCollisionBody(Object* object);

		float X() const override;
		float Y() const override;
		void SetX(float x) override;
		void SetY(float y) override;
		PointF Position() const override;
		void SetPosition(const PointF& position) override;
		void SetPosition(float x, float y) override;
		RectangleF AABB() const override;
		CollisionFilter& Filter() override;
		const CollisionFilter& Filter() const override;
		HitMaskPtr& HitMask() override;
		const HitMaskPtr& HitMask() const override;
		void SetHitMask(HitMaskPtr& mask) override;
		virtual bool IsSolid() const override;
		virtual void SetSolid(bool value) override;

		Object* GetObject();

	private:
		bool _solid;
		Object* _object;
		HitMaskPtr _mask;
		CollisionFilter _filter;

	};

};