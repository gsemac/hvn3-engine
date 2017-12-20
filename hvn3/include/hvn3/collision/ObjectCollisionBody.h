#pragma once
#include "hvn3/collision/CollisionBodyBase.h"

namespace hvn3 {

	class Object;

	class ObjectCollisionBody final : public CollisionBodyBase {

	public:
		ObjectCollisionBody(Object* object);
		ObjectCollisionBody(ObjectCollisionBody&& other);

		float X() const override;
		float Y() const override;
		PointF Position() const override;
		void SetX(float x) override;
		void SetY(float y) override;
		void SetPosition(const PointF& position) override;

		Object* GetObject();
		const Object* GetObject() const;

	private:
		Object* _object;

	};

};