#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/io/PlatformerControls.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/objects/Object.h"

namespace hvn3 {

	PlatformerControls::PlatformerControls(Object* object, float speed, int platform_category_bits) :
		DirectionalControls(2, speed),
		_gravity(0.0f, 16.0f) {

		_object = object;
		_platform_category_bits = platform_category_bits;

		_jump_height = 32.0f;
		_step_height = 0.0f;
		_climb_height = 0.0f;
		_is_grounded = false;
		_friction = speed / 10.0f; // Friction is stored separately from the base class so it can be applied only when grounded.

		SetAcceleration(speed / 10.0f);
		SetFriction(_friction);

	}

	float PlatformerControls::Friction() const {
		return _friction;
	}
	void PlatformerControls::SetFriction(float value) {
		_friction = value;
	}
	float PlatformerControls::JumpHeight() const {
		return _jump_height;
	}
	void PlatformerControls::SetJumpHeight(float value) {
		_jump_height = value;
	}
	float PlatformerControls::StepHeight() const {
		return _step_height;
	}
	void PlatformerControls::SetStepHeight(float value) {
		_step_height = value;
	}
	float PlatformerControls::ClimbHeight() const {
		return _climb_height;
	}
	void PlatformerControls::SetClimbHeight(float value) {
		_climb_height = value;
	}
	const Vector2d& PlatformerControls::Gravity() const {
		return _gravity;
	}
	void PlatformerControls::SetGravity(const Vector2d& value) {
		_gravity = value;
	}
	bool PlatformerControls::IsGrounded() const {
		return _is_grounded;
	}

	void PlatformerControls::Step() {
		Step(1.0f);
	}
	void PlatformerControls::Step(double delta) {

		DirectionalControls::Step();

		SetVelocity(Velocity() + _gravity);

		float deltaf = static_cast<float>(delta);
		auto body = _object->GetCollisionBody();

		float xvelf = Velocity().X() * deltaf;
		Vector2d xvel(Velocity().X() * deltaf, 0.0f);
		PointF pstart = _object->Position();
		bool moving_up_slope = false;

		if (xvel.X() != 0.0f && _object->Context().GetCollisions().MoveContact(body, xvel.Direction(), xvel.Length(), _platform_category_bits)) {

			// We have hit an obstacle.
			// Calculate the total horizontal distance traveled, and subtract it from the current velocity.
			float xdist = _object->X() - pstart.x;
			xvel.SetX(xvel.X() - xdist);

			// Attempt to move up the obstacle if possible (i.e. slope movement).
			// Only attempt to move up a slope if we're grounded.
			if (Velocity().Y() >= 0.0f) {
				for (float try_height = 0; try_height <= _step_height; try_height += 1.0f) {
					// Otherwise, see if there is a slope we can climb.
					xvel.SetY(-try_height);
					PointF new_pos = pstart + xvel;
					if (_object->Context().GetCollisions().PlaceFree(body, new_pos, _platform_category_bits)) {
						_object->SetPosition(new_pos);
						moving_up_slope = true;
						break;
					}
				}
				// If we weren't able to walk up a slope, try going straight up vertically. This is necessary for steeper slopes.
				if (!moving_up_slope && _climb_height > 0.0f) {
					if (_object->Context().GetCollisions().PlaceFree(body, pstart + Vector2d(xvel.X(), -_climb_height), _platform_category_bits)) {
						_object->SetPosition(pstart + Vector2d(0.0f, -Math::Abs(xvel.X())));
						moving_up_slope = true;
						SetVelocity(Velocity() - _gravity);
					}
				}
			}

			// We can't move any further, so clear the horizontal velocity.
			if (!moving_up_slope)
				SetVelocity(Vector2d(0.0f, Velocity().Y()));

		}

		Vector2d yvel(0.0f, Velocity().Y() * deltaf);

		// Moving down slopes can appear jumpy if the horizontal velocity is greater than the force of gravity.
		// For insignificant horizontal velocity, "snap" to the floor.
		if (_step_height > 0.0f && _is_grounded && !_object->Context().GetCollisions().PlaceFree(body, _object->Position() + Vector2d(yvel.X(), yvel.Y() + _step_height), _platform_category_bits))
			yvel.SetY(yvel.Y() + _step_height);

		if (yvel.Y() != 0.0f && _object->Context().GetCollisions().MoveContact(body, yvel.Direction(), yvel.Length(), _platform_category_bits)) {
			if (Velocity().Y() >= 0.0f)
				_setGrounded(true);
			else
				_setGrounded(false);
			SetVelocity(Vector2d(Velocity().X(), 0.0f));
		}

	}

	void PlatformerControls::OnKeyPressed(KeyPressedEventArgs& e) {

		DirectionalControls::OnKeyPressed(e);

		if (e.Key() == GetKeyData(KEYDIR_UP).key && _is_grounded) {
			SetVelocity(Vector2d(Velocity().X(), -(_gravity.Y() + _jump_height) * 6.0f));
			_setGrounded(false);
		}

	}



	void PlatformerControls::_setGrounded(bool value) {
		_is_grounded = value;
		if (!_is_grounded)
			DirectionalControls::SetFriction(0.0f);
		else
			DirectionalControls::SetFriction(_friction);
	}

}