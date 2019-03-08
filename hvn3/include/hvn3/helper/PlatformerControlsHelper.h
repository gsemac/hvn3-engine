#pragma once
#include "hvn3/core/ApplicationContext.h"
#include "hvn3/helper/DirectionalControlsHelper.h"
#include "hvn3/utility/BitFlags.h"

namespace hvn3 {

	class ICollider;
	class IObject;

	enum class PlatformerControlsHelperFlags {
		SnapOnDownslope = 1,
		EasySlopes = SnapOnDownslope | 2
	};
	ENABLE_BITFLAG_OPERATORS(PlatformerControlsHelperFlags)

	class PlatformerControlsHelper : public DirectionalControlsHelper {

	public:
		PlatformerControlsHelper(IObject* object, ICollider* collider, float speed, int platform_category_bits);

		float Friction() const;
		void SetFriction(float value);
		float JumpHeight() const;
		void SetJumpHeight(float value);
		float StepHeight() const;
		void SetStepHeight(float value);
		float ClimbHeight() const;
		void SetClimbHeight(float value);
		const Vector2d& Gravity() const;
		void SetGravity(const Vector2d& value);
		bool IsGrounded() const;
		PlatformerControlsHelperFlags Flags() const;
		void SetFlags(PlatformerControlsHelperFlags value);

		void Step(const ApplicationContext& context);
		void Step(const ApplicationContext& context, double delta);

		void OnKeyPressed(KeyPressedEventArgs& e) override;

	private:
		void _setGrounded(bool value);

		IObject* _object;
		ICollider* _collider;
		int _platform_category_bits;
		float _jump_height, _step_height, _climb_height;
		float _friction;
		Vector2d _gravity;
		bool _is_grounded;
		PlatformerControlsHelperFlags _flags;

	};

}