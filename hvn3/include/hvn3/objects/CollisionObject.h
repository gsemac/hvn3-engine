//#pragma once
//#include "hvn3/collision/ObjectCollisionBody.h"
//#include "hvn3/objects/Object.h"
//#include "hvn3/objects/ObjectTypeDefs.h"
//
//namespace hvn3 {
//
//	class CollisionObject : public Object {
//
//	public:
//		CollisionObject(ObjectId id, float x, float y);
//		CollisionObject(ObjectId id, const PointF& position);
//
//		void OnCreate(CreateEventArgs& e) override;
//
//		ICollisionBody& CollisionBody();
//		const ICollisionBody& CollisionBody() const;
//
//	private:
//		ObjectCollisionBody _collision_body;
//
//	};
//
//}