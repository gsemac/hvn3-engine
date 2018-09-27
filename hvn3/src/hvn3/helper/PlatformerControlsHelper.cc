#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/collision/ICollider.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/helper/PlatformerControlsHelper.h"
#include "hvn3/io/KeyboardEventArgs.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/objects/Object.h"

namespace hvn3 {

	PlatformerControlsHelper::PlatformerControlsHelper(Object* object, ICollider* collider, float speed, int platform_category_bits) :
		DirectionalControlsHelper(2, speed),
		_gravity(0.0f, 512.0f) {

		_object = object;
		_collider = collider;
		_platform_category_bits = platform_category_bits;
		_flags = static_cast<PlatformerControlsHelperFlags>(0);

		_jump_height = 192.0f;
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
	PlatformerControlsHelperFlags PlatformerControlsHelper::Flags() const {
		return _flags;
	}
	void PlatformerControlsHelper::SetFlags(PlatformerControlsHelperFlags value) {
		_flags = value;
	}

	void PlatformerControlsHelper::Step(const Context& context) {
		Step(context, 1.0f);
	}
	void PlatformerControlsHelper::Step(const Context& context, double delta) {

		DirectionalControlsHelper::Step();

		auto body = _collider;

		float deltaf = static_cast<float>(delta);
		float xvel = Velocity().X();
		float yvel = Velocity().Y();
		float xdir = static_cast<float>(xvel < 0.0f ? DIRECTION_LEFT : DIRECTION_RIGHT);
		float ydir = static_cast<float>(yvel < 0.0f ? DIRECTION_UP : DIRECTION_DOWN);

		// Note that at higher framerates, gravity will be applied more often, and therefore needs to be scaled immediately.
		yvel += _gravity.Y() * deltaf;

		// Update horizontal position.

		if (xvel != 0.0f) {

			float xmax = Math::Abs(xvel) * deltaf;
			float xleft = xmax;
			PointF pprev = _object->Position();
			bool slope_success = true;

			while (xleft > 0.0f) {

				//// Check for downward slopes.
				//while (!Math::IsZero(xleft)) {

				//	PointF ptry = pprev + PointF(Math::Min(1.0f, xleft) * Math::Sign(xvel), 1.0f);

				//	if (_object->Context().GetCollisions().PlaceFree(body, ptry, _platform_category_bits)) {

				//		_object->SetPosition(ptry);
				//		xleft -= Math::Geometry::PointDistance(ptry, pprev);

				//		break;

				//	}
				//	else
				//		slope_success = false;

				//	if (!slope_success)
				//		break;

				//}

				//if (xleft < 0.0f || Math::IsZero(xleft))
				//	break;

				// Handle normal horizontal movement.

				context.Get<COLLISION_MANAGER>().MoveContact(body, xdir, xmax, _platform_category_bits);

				xleft -= Math::Abs(pprev.x - _object->X());
				pprev = _object->Position();

				if (xleft < 0.0f || Math::IsZero(xleft))
					break;

				// Check for upward slopes.

				slope_success = false;

				if (yvel >= 0.0f) {

					for (float i = 0.0f; i <= _step_height; i += 1.0f) {

						PointF ptry = pprev + PointF(Math::Min(1.0f, xleft) * Math::Sign(xvel), -i);

						if (context.Get<COLLISION_MANAGER>().PlaceFree(body, ptry, _platform_category_bits)) {

							_object->SetPosition(ptry);
							xleft -= Math::Geometry::PointDistance(ptry, pprev);

							slope_success = true;

							break;

						}

					}

				}

				if (!slope_success) {
					// Hit a wall.
					xvel = 0.0f;
					break;
				}

			}

		}

		// Update vertical position.

		if (context.Get<COLLISION_MANAGER>().MoveContact(body, ydir, Math::Abs(yvel) * deltaf, _platform_category_bits)) {
			yvel = 0.0f;
			_setGrounded(true);
		}
		else {
			_setGrounded(false);
		}

		// Update the velocity for the next update.
		SetVelocity(Vector2d(xvel, yvel));

	}

	void PlatformerControlsHelper::OnKeyPressed(KeyPressedEventArgs& e) {

		DirectionalControlsHelper::OnKeyPressed(e);

		if (e.Key() == GetKeyData(KEYDIR_UP).key && _is_grounded) {
			SetVelocity(Vector2d(Velocity().X(), -_jump_height));
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