#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/helper/PlatformerControlsHelper.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/objects/Object.h"

namespace hvn3 {

	PlatformerControlsHelper::PlatformerControlsHelper(Object* object, float speed, int platform_category_bits) :
		DirectionalControlsHelper(2, speed),
		_gravity(0.0f, 16.0f) {

		_object = object;
		_platform_category_bits = platform_category_bits;
		_flags = static_cast<PlatformerControlsFlags>(0);

		_jump_height = 32.0f;
		_step_height = 0.0f;
		_climb_height = 0.0f;
		_is_grounded = false;
		_friction = speed / 10.0f; // Friction is stored separately from the base class so it can be applied only when grounded.

		SetAcceleration(speed / 10.0f);
		SetFriction(_friction);

	}

	float PlatformerControlsHelper::Friction() const {
		return _friction;
	}
	void PlatformerControlsHelper::SetFriction(float value) {
		_friction = value;
	}
	float PlatformerControlsHelper::JumpHeight() const {
		return _jump_height;
	}
	void PlatformerControlsHelper::SetJumpHeight(float value) {
		_jump_height = value;
	}
	float PlatformerControlsHelper::StepHeight() const {
		return _step_height;
	}
	void PlatformerControlsHelper::SetStepHeight(float value) {
		_step_height = value;
	}
	float PlatformerControlsHelper::ClimbHeight() const {
		return _climb_height;
	}
	void PlatformerControlsHelper::SetClimbHeight(float value) {
		_climb_height = value;
	}
	const Vector2d& PlatformerControlsHelper::Gravity() const {
		return _gravity;
	}
	void PlatformerControlsHelper::SetGravity(const Vector2d& value) {
		_gravity = value;
	}
	bool PlatformerControlsHelper::IsGrounded() const {
		return _is_grounded;
	}
	PlatformerControlsFlags PlatformerControlsHelper::Flags() const {
		return _flags;
	}
	void PlatformerControlsHelper::SetFlags(PlatformerControlsFlags value) {
		_flags = value;
	}

	void PlatformerControlsHelper::Step() {
		Step(1.0f);
	}
	void PlatformerControlsHelper::Step(double delta) {
		DirectionalControlsHelper::Step();

		// Update the vertical velocity according to the gravity.
		SetVelocity(Velocity() + _gravity);

		// Initialize some helper variables.
		float deltaf = static_cast<float>(delta);
		auto body = _object->GetCollisionBody();
		Vector2d xvel(Velocity().X() * deltaf, 0.0f);
		Vector2d yvel(0.0f, Velocity().Y() * deltaf);
		PointF pstart = _object->Position();
		bool moving_up_slope = false;

		// Apply horizontal velocity. If this puts us in contact with a wall, stop all horizontal velocity.
		if (xvel.X() != 0.0f && _object->Context().GetCollisions().MoveContact(body, xvel.Direction(), xvel.Length(), _platform_category_bits)) {

			if (HasFlag(_flags, PlatformerControlsFlags::EasySlopes)) {

				// If we've hit an obstacle and slopes are enabled, this is a chance to check for possible movement. Start by adjusting the horizontal velocity to store the remainder.
				xvel.SetX(xvel.X() - (_object->X() - pstart.x));

				if (Velocity().Y() >= 0.0f) {

					// The first option is to attempt to climb the slope according to the maximum step height. If it's possible to climb the slope, the object's position is updated.
					for (float try_height = 1.0f; try_height <= _step_height; try_height += 1.0f) {
						PointF new_pos = pstart + Vector2d(xvel.X(), -try_height);
						if (_object->Context().GetCollisions().PlaceFree(body, new_pos, _platform_category_bits)) {
							_object->SetPosition(new_pos);
							moving_up_slope = true;
							break;
						}
					}

					// For steeper slopes, we can try "climbing" them, which involves gradual vertical movement until the barrier has been overcome.
					if (!moving_up_slope && _climb_height > 0.0f) {
						if (_object->Context().GetCollisions().PlaceFree(body, pstart + Vector2d(xvel.X(), -_climb_height), _platform_category_bits)) {
							_object->SetPosition(pstart + Vector2d(0.0f, -Math::Abs(xvel.X())));
							SetVelocity(Velocity() - _gravity);
							moving_up_slope = true;
						}
					}

				}

			}

			// If we've hit a wall and weren't able to move up the slope, stop all horizontal velocity.
			if (!moving_up_slope)
				SetVelocity(Vector2d(0.0f, Velocity().Y()));

		}

		// Moving down slopes can appear jumpy if the horizontal velocity is greater than the force of gravity. For insignificant horizontal velocity, "snap" to the floor.
		if (HasFlag(_flags, PlatformerControlsFlags::EasySlopes) && HasFlag(_flags, PlatformerControlsFlags::SnapOnDownslope)) {
			if (_step_height > 0.0f && _is_grounded && !_object->Context().GetCollisions().PlaceFree(body, pstart + Vector2d(Math::Sign(yvel.X()) * 1.0f, 1.0f), _platform_category_bits))
				yvel.SetY(yvel.Y() + _step_height);
		}

		// Apply vertical velocity. If this puts us in contact with the ground, set the 'grounded' flag to true, and stop all vertical velocity.
		if (yvel.Y() != 0.0f && _object->Context().GetCollisions().MoveContact(body, yvel.Direction(), yvel.Length(), _platform_category_bits)) {
			if (Velocity().Y() >= 0.0f)
				_setGrounded(true);
			else
				_setGrounded(false);
			SetVelocity(Vector2d(Velocity().X(), 0.0f));
		}

	}

	void PlatformerControlsHelper::OnKeyPressed(KeyPressedEventArgs& e) {

		DirectionalControlsHelper::OnKeyPressed(e);

		if (e.Key() == GetKeyData(KEYDIR_UP).key && _is_grounded) {
			SetVelocity(Vector2d(Velocity().X(), -(_gravity.Y() + _jump_height) * 6.0f));
			_setGrounded(false);
		}

	}



	void PlatformerControlsHelper::_setGrounded(bool value) {
		_is_grounded = value;
		if (!_is_grounded)
			DirectionalControlsHelper::SetFriction(0.0f);
		else
			DirectionalControlsHelper::SetFriction(_friction);
	}

}