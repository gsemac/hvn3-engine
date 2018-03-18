#pragma once
#include "hvn3/io/DirectionalKeyboardController.h"

namespace hvn3 {

	class Object;

	class PlatformerKeyboardController : public DirectionalKeyboardController {

	public:
		PlatformerKeyboardController(Object* object, float speed, int platform_category_bits);

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

		void Step();
		void Step(double delta);

		void OnKeyPressed(KeyPressedEventArgs& e) override;

	private:
		void _setGrounded(bool value);

		Object* _object;
		int _platform_category_bits;
		float _jump_height;
		float _step_height, _climb_height;
		float _friction;
		Vector2d _gravity;
		bool _is_grounded;

	};

}