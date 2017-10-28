#pragma once
#include "collision/ICollisionBody.h"
#include "objects/Object.h"

namespace hvn3 {
	namespace Collision {

		class ObjectCollisionBody final : public ICollisionBody {

		public:
			ObjectCollisionBody(ObjectPtr& object);

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
			const Object* GetObject() const;
			bool ObjectExpired() const;

		private:
			bool _solid;
			Object* _key;
			std::weak_ptr<Object> _object;
			HitMaskPtr _mask;
			CollisionFilter _filter;

		};

	}
};