#pragma once
#include "hvn3/objects/ObjectBase.h"
#include "hvn3/core/SharedCreatableBase.h"
#include "hvn3/core/Positionable2dBase.h"
#include "hvn3/collision/ICollidable.h"

namespace hvn3 {

	class Object : 
		public ICollidable, 
		public ObjectBase, 
		public Positionable2dBase<float> {

	public:
		Object(ObjectId id);
		Object(ObjectId id, const PointF& position);
		Object(ObjectId id, float x, float y);

		ObjectId Id() const;

		int Depth() const;
		void SetDepth(int depth);
		
		virtual void OnCreate(CreateEventArgs& e) override;
		virtual void OnDestroy(DestroyEventArgs& e) override;
		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnCollision(CollisionEventArgs& e) override;

	private:
		ObjectId _id;
		int _depth;

	};

}