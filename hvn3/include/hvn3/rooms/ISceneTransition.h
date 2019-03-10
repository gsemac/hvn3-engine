#pragma once

#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"

namespace hvn3 {

	enum class SceneTransition {
		None,
		Fade
	};

	class ISceneTransition {

	public:
		virtual ~ISceneTransition() = default;

		virtual void ExitBegin() = 0;
		virtual bool ExitStep(UpdateEventArgs& e) = 0;
		virtual void ExitEnd() = 0;

		virtual void EnterBegin() = 0;
		virtual bool EnterStep(UpdateEventArgs& e) = 0;
		virtual void EnterEnd() = 0;

		virtual bool Blocking() const = 0;

		virtual void OnDraw(DrawEventArgs& e) = 0;

	};

}