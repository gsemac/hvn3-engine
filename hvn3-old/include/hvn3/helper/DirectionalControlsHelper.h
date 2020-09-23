//#pragma once
//#include "hvn3/helper/DirectionalInputListener.h"
//#include "hvn3/math/Vector2d.h"
//
//namespace hvn3 {
//
//	class DirectionalControlsHelper : 
//		public DirectionalInputListener {
//
//	public:
//		DirectionalControlsHelper(int directions, float speed);
//
//		float Acceleration() const;
//		void SetAcceleration(float value);
//		float Friction() const;
//		void SetFriction(float value);
//		float MaxSpeed() const;
//		void SetMaxSpeed(float value);
//		const Vector2d& Velocity() const;
//
//		void Step();
//		void Clear();
//
//	protected:
//		void SetVelocity(const Vector2d& value);
//
//	private:
//		float _acceleration;
//		float _friction;
//		float _max_speed;
//		Vector2d _velocity;
//
//	};
//
//}