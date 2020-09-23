#pragma once

#include "hvn3/rooms/IScene.h"

namespace hvn3::scenes {

	class SceneBase :
		public IScene {

	public:
		SceneBase();

		bool IsPersistent() const override;
		void SetPersistent(bool value) override;

	protected:
		void OnEnter(EnterEventArgs& e) override;
		void OnExit(ExitEventArgs& e) override;

	private:
		bool persistent;

	};

}