#pragma once
#include "hvn3/collision/ColliderBase.h"

namespace hvn3 {

	class IObject;

	class Collider : 
		public ColliderBase {

	public:
		Collider(IObject* object);
		Collider(Collider&& other);

		float X() const override;
		float Y() const override;
		PointF Position() const override;
		void SetX(float x) override;
		void SetY(float y) override;
		void SetPosition(const PointF& position) override;
		void OnCollide(ICollider* other) const override;

	private:
		IObject* _object;

	};

};