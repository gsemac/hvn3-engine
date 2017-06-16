#pragma once
#include "ObjectBase.h"
#include "SharedCreatableBase.h"
#include "Positionable2dBase.h"
#include "Collider.h"
#include "ICollidable.h"

namespace hvn3 {

	class Object : public ICollidable, public ObjectBase, public Positionable2dBase<float>, public SharedCreateableBase<Object> {

	public:
		Object(ObjectId id);
		Object(ObjectId id, const PointF& position);

		ObjectId Id() const;

		int Depth() const;
		void SetDepth(int depth);
		
		hvn3::Collider& Collider();
		const hvn3::Collider& Collider() const override;

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnCollision(CollisionEventArgs& e) override;

	private:
		ObjectId _id;
		int _depth;
		hvn3::Collider _collider;

	};

}