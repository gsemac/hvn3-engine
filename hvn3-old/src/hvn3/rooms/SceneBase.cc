#include "hvn3/rooms/SceneBase.h"

namespace hvn3::scenes {

	// Public members

	SceneBase::SceneBase() :
		persistent(false) {
	}

	bool SceneBase::IsPersistent() const {

		return persistent;

	}
	void SceneBase::SetPersistent(bool value) {

		persistent = value;

	}

	// Protected members

	void SceneBase::OnEnter(EnterEventArgs& e) {}
	void SceneBase::OnExit(ExitEventArgs& e) {}

}