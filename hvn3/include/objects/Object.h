#pragma once
#include "objects/ObjectBase.h"
#include "core/SharedCreatableBase.h"
#include "core/Positionable2dBase.h"
#include "collision/ICollidable.h"

namespace hvn3 {

	typedef std::shared_ptr<Object> ObjectPtr;
	typedef std::shared_ptr<const Object> ConstObjectPtr;

	class Object : 
		public ICollidable, 
		public ObjectBase, 
		public Positionable2dBase<float> {

	public:
		Object(ObjectId id);
		Object(ObjectId id, const PointF& position);
		virtual ~Object() = default;

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