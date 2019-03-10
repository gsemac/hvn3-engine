#include "hvn3/rooms/SceneTransitionBase.h"

namespace hvn3 {

	void SceneTransitionBase::ExitBegin() {}
	bool SceneTransitionBase::ExitStep(UpdateEventArgs& e) {

		return true;

	}
	void SceneTransitionBase::ExitEnd() {}

	void SceneTransitionBase::EnterBegin() {}
	bool SceneTransitionBase::EnterStep(UpdateEventArgs& e) {

		return true;

	}
	void SceneTransitionBase::EnterEnd() {}

	void SceneTransitionBase::OnDraw(DrawEventArgs& e) {}

	bool SceneTransitionBase::Blocking() const {

		return false;

	}

}