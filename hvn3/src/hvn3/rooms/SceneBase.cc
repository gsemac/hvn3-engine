#include "hvn3/rooms/SceneBase.h"

namespace hvn3 {

	// SceneBase

	// Public methods

	SceneBase::SceneBase() :
		_persistent(false),
		_background_color(Color::Silver) {
	}

	bool SceneBase::IsPersistent() const {

		return _persistent;

	}
	void SceneBase::SetPersistent(bool value) {

		_persistent = value;

	}
	Color SceneBase::BackgroundColor() const {

		return _background_color;

	}
	void SceneBase::SetBackgroundColor(const Color& color) {

		_background_color = color;

	}

	// Protected methods

	void SceneBase::OnCreate(CreateEventArgs& e) {}
	void SceneBase::OnEnter(EnterEventArgs& e) {}
	void SceneBase::OnExit(ExitEventArgs& e) {}

}