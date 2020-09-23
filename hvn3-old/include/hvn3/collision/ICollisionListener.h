#pragma once

namespace hvn3 {

	class ICollisionListener {

	public:
		class CollisionEventArgs {};

		virtual ~ICollisionListener() = default;

		virtual void OnCollision(CollisionEventArgs& e) = 0;

	};

}