#pragma once
#include "CollisionMask.h"
#include "CollisionFilter.h"

namespace hvn3 {

	class RoomBase;

	class ICollidable : public IPositionable2d<float> {

	public:
		ICollidable(float x, float y);

		Rectangle<float> AABB() const;
		CollisionMask& CollisionMask();
		void SetCollisionMask(const hvn3::CollisionMask& mask);
		CollisionFilter& Filter();

		//// Returns a pointer to the Scene the object belongs to, or nullptr if the object does not belong to a Scene.
		//Room& Scene();

		bool CollidesWith(const ICollidable* other) const;
		virtual void Collide(ICollidable* other);

	private:
		hvn3::CollisionMask __mask;
		CollisionFilter __filter;
		//::Room* __scene;

	};

}