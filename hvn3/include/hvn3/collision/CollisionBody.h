#pragma once
#include "hvn3/collision/CollisionBodyBase.h"

namespace hvn3 {

	class IObject;

	class CollisionBody : public CollisionBodyBase {

	public:
		CollisionBody(IObject* object);
		CollisionBody(CollisionBody&& other);

		float X() const override;
		float Y() const override;
		PointF Position() const override;
		void SetX(float x) override;
		void SetY(float y) override;
		void SetPosition(const PointF& position) override;

		IObject* GetObject();

	private:
		IObject* _object;

	};

};