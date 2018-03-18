#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/io/PlatformerKeyboardController.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/objects/Object.h"

namespace hvn3 {

	PlatformerKeyboardController::PlatformerKeyboardController(Object* object, float speed, int platform_category_bits) :
		DirectionalKeyboardController(2, speed),
		_gravity(0.0f, 16.0f) {

		_object = object;
		_platform_category_bits = platform_category_bits;

		_jump_height = 32.0f;
		_step_height = 0.0f;
		_climb_height = 0.0f;
		_is_grounded = false;
		_friction = speed / 15.0f; // Friction is stored separately from the base class so it can be applied only when grounded.

		SetAcceleration(speed / 10.0f);
		SetFriction(_friction);

	}

	float PlatformerKeyboardController::Friction() const {
		return _friction;
	}
	void PlatformerKeyboardController::SetFriction(float value) {
		_friction = value;
	}
	float PlatformerKeyboardController::JumpHeight() const {
		return _jump_height;
	}
	void PlatformerKeyboardController::SetJumpHeight(float value) {
		_jump_height = value;
	}
	float PlatformerKeyboardController::StepHeight() const {
		return _step_height;
	}
	void PlatformerKeyboardController::SetStepHeight(float value) {
		_step_height = value;
	}
	float PlatformerKeyboardController::ClimbHeight() const {
		return _climb_height;
	}
	void PlatformerKeyboardController::SetClimbHeight(float value) {
		_climb_height = value;
	}
	const Vector2d& PlatformerKeyboardController::Gravity() const {
		return _gravity;
	}
	void PlatformerKeyboardController::SetGravity(const Vector2d& value) {
		_gravity = value;
	}

	void PlatformerKeyboardController::Step() {
		Step(1.0f);
	}
	void PlatformerKeyboardController::Step(double delta) {

		DirectionalKeyboardController::Step();

		SetVelocity(Velocity() + _gravity);

		float deltaf = static_cast<float>(delta);

		Vector2d xvel(Velocity().X() * deltaf, 0.0f);
		Vector2d yvel(0.0f, Velocity().Y() * deltaf);
		PointF pstart = _object->Position();

		if (xvel.X() != 0.0f && _object->Context().GetCollisions().MoveContact(_object->GetCollisionBody(), xvel.Direction(), xvel.Length(), _platform_category_bits)) {
			// We have hit an obstacle.
			// Calculate the total horizontal distance traveled, and subtract it from the current velocity.
			float xdist = _object->X() - pstart.x;
			xvel.SetX(xvel.X() - xdist);
			// If a step height has been set and we can still move horizontally, attempt to move up the slope.
			bool slope_success = false;
			for (float try_height = 0; try_height <= _step_height; try_height += 1.0f) {
				// Otherwise, see if there is a slope we can climb.
				xvel.SetY(-try_height);
				PointF new_pos = _object->Position() + xvel;
				if (_object->Context().GetCollisions().PlaceFree(_object->GetCollisionBody(), new_pos, _platform_category_bits)) {
					_object->SetPosition(new_pos);
					slope_success = true;
					break;
				}
			}
			// If we weren't able to walk up a slope, try going straight up vertically. This is necessary for steeper slopes.
			if (!slope_success && _climb_height > 0.0f) {
				if (_object->Context().GetCollisions().PlaceFree(_object->GetCollisionBody(), _object->Position() + Vector2d(xvel.X(), -_climb_height), _platform_category_bits)) {
					_object->SetPosition(_object->Position() + Vector2d(0.0f, -Math::Abs(xvel.X())));
					slope_success = true;
					SetVelocity(Velocity() - _gravity);
				}
			}
			// We can't move any further, so clear the horizontal velocity.
			if (!slope_success)
				SetVelocity(Vector2d(0.0f, Velocity().Y()));
		}

		if (yvel.Y() != 0.0f && _object->Context().GetCollisions().MoveContact(_object->GetCollisionBody(), yvel.Direction(), yvel.Length(), _platform_category_bits)) {
			if (Velocity().Y() >= 0.0f)
				_setGrounded(true);
			else
				_setGrounded(false);
			SetVelocity(Vector2d(Velocity().X(), 0.0f));
		}

	}

	void PlatformerKeyboardController::OnKeyPressed(KeyPressedEventArgs& e) {

		DirectionalKeyboardController::OnKeyPressed(e);

		if (e.Key() == GetKeyData(KEYDIR_UP).key && _is_grounded) {
			SetVelocity(Vector2d(Velocity().X(), -(_gravity.Y() + _jump_height) * 6.0f));
			_setGrounded(false);
		}

	}



	void PlatformerKeyboardController::_setGrounded(bool value) {
		_is_grounded = value;
		if (!_is_grounded)
			DirectionalKeyboardController::SetFriction(0.0f);
		else
			DirectionalKeyboardController::SetFriction(_friction);
	}

}