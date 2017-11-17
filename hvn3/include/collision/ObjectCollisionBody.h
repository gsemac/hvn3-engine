#pragma once
#include "collision/CollisionBodyBase.h"
#include "objects/Object.h"

namespace hvn3 {

	class ObjectCollisionBody final : public CollisionBodyBase {

	public:
		ObjectCollisionBody(ObjectPtr& object);
		ObjectCollisionBody(ObjectCollisionBody&& other);

		float X() const override;
		float Y() const override;
		void SetX(float x) override;
		void SetY(float y) override;

		Object* GetObject();
		const Object* GetObject() const;
		bool ObjectExpired() const;

		bool IsDestroyed() const override;

	private:
		Object* _key;
		std::weak_ptr<Object> _object;

	};

};