#pragma once
#include "hvn3/io/KeyboardListener.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/utility/Direction8.h"

namespace hvn3 {

	class DirectionalControls : public KeyboardListener {

	public:
		DirectionalControls(int directions, float speed);

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
		void SetKeys(Key up, Key left, Key down, Key right);

		void Step();
		void Clear();

	protected:
		enum KEYDIR {
			KEYDIR_UP,
			KEYDIR_DOWN,
			KEYDIR_LEFT,
			KEYDIR_RIGHT
		};
		struct KeyData {
			Key key;
			bool pressed;
		};


		KeyData GetKeyData(KEYDIR keydir);
		void SetVelocity(const Vector2d& value);

	private:
		void _resetKeyStates();

		int _directions;
		float _acceleration;
		float _friction;
		float _max_speed;
		Vector2d _velocity;
		DIRECTION _last_hdir;
		DIRECTION _last_vdir;
		KeyData _keys[4];

	};

}