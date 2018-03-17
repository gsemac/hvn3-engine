#pragma once
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/utility/Direction8.h"

namespace hvn3 {

	class DirectionalKeyboardController : public KeyboardListener {

		enum KEYDIR {
			KEYDIR_UP,
			KEYDIR_DOWN,
			KEYDIR_LEFT,
			KEYDIR_RIGHT
		};

	public:
		DirectionalKeyboardController(float speed);
		DirectionalKeyboardController(float speed, int directions);

		void OnKeyDown(KeyDownEventArgs& e) override;
		void OnKeyPressed(KeyPressedEventArgs& e) override;
		void OnKeyUp(KeyUpEventArgs& e) override;

		float Acceleration() const;
		void SetAcceleration(float value);
		float Friction() const;
		void SetFriction(float value);
		float MaxSpeed() const;
		void SetMaxSpeed(float value);
		const Vector2d& Velocity() const;
		void SetKeys(Key left, Key right, Key up, Key down);

		void Step();
		void Step(double delta);
		void Clear();

	private:
		void _resetKeyStates();

		int _directions;
		float _acceleration;
		float _friction;
		float _max_speed;
		Vector2d _velocity;
		DIRECTION _last_hdir;
		DIRECTION _last_vdir;
		struct {
			Key key;
			bool pressed;
		} _keys[4];

	};

}