#include "hvn3/rooms/SceneBase.h"

namespace hvn3 {

	// SceneBase

	// Public methods

	SceneBase::SceneBase() :
		_persistent(false) {
	}

	bool SceneBase::IsPersistent() const {

		return _persistent;

	}
	void SceneBase::SetPersistent(bool value) {

		_persistent = value;

	}

	// Protected methods

	void SceneBase::OnEnter(EnterEventArgs& e) {}
	void SceneBase::OnExit(ExitEventArgs& e) {}

}