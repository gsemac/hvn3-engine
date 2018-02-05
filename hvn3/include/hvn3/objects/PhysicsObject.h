//#pragma once
//#include "hvn3/objects/CollisionObject.h"
//#include "hvn3/physics/PhysicsBodyBase.h"
//
//namespace hvn3 {
//
//	class PhysicsObject : public CollisionObject {
//
//	public:
//		PhysicsObject(ObjectId id, float x, float y);
//		PhysicsObject(ObjectId id, const PointF& position);
//
//		void OnCreate(CreateEventArgs& e) override;
//
//		Physics::IPhysicsBody& PhysicsBody();
//		const Physics::IPhysicsBody& PhysicsBody() const;
//
//	private:
//		Physics::PhysicsBodyBase _physics_body;
//
//	};
//
//}