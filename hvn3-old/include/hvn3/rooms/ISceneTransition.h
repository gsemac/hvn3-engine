#pragma once

#include "hvn3/events/DrawEventArgs.h"
#include "hvn3/events/UpdateEventArgs.h"

namespace hvn3 {

	enum class SceneTransition {
		None,
		Fade
	};

	class ISceneTransition {

	public:
		typedef UpdateEventArgs ExitStepEventArgs;
		typedef UpdateEventArgs EnterStepEventArgs;

		virtual ~ISceneTransition() = default;

		virtual void OnExitBegin() = 0;
		virtual bool OnExitStep(ExitStepEventArgs& e) = 0;
		virtual void OnExitEnd() = 0;

		virtual void OnEnterBegin() = 0;
		virtual bool OnEnterStep(EnterStepEventArgs& e) = 0;
		virtual void OnEnterEnd() = 0;

		virtual bool Blocking() const = 0;

		virtual void OnDraw(DrawEventArgs& e) = 0;

	};

}