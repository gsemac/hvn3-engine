#include "hvn3/rooms/SceneTransitionBase.h"

namespace hvn3 {

	void SceneTransitionBase::OnExitBegin() {}
	bool SceneTransitionBase::OnExitStep(ISceneTransition::ExitStepEventArgs& e) {

		return true;

	}
	void SceneTransitionBase::OnExitEnd() {}

	void SceneTransitionBase::OnEnterBegin() {}
	bool SceneTransitionBase::OnEnterStep(ISceneTransition::EnterStepEventArgs& e) {

		return true;

	}
	void SceneTransitionBase::OnEnterEnd() {}

	void SceneTransitionBase::OnDraw(DrawEventArgs& e) {}

	bool SceneTransitionBase::Blocking() const {

		return false;

	}

}