#pragma once

#include "hvn3/rooms/ISceneTransition.h"

namespace hvn3 {

	class SceneTransitionBase :
		public ISceneTransition {

	public:
		void OnExitBegin() override;
		bool OnExitStep(ExitStepEventArgs& e) override;
		void OnExitEnd() override;

		void OnEnterBegin()override;
		bool OnEnterStep(EnterStepEventArgs& e) override;
		void OnEnterEnd() override;

		bool Blocking() const override;

		void OnDraw(DrawEventArgs& e) override;

	};

	typedef SceneTransitionBase SceneTransitionNone;

}