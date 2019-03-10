#pragma once

#include "hvn3/rooms/IScene.h"

namespace hvn3 {

	class SceneBase :
		public IScene {

	public:
		SceneBase();

		bool IsPersistent() const override;
		void SetPersistent(bool value) override;
		Color BackgroundColor() const override;
		void SetBackgroundColor(const Color& color) override;

	protected:
		void OnCreate(CreateEventArgs& e) override;
		void OnEnter(EnterEventArgs& e) override;
		void OnExit(ExitEventArgs& e) override;

	private:
		bool _persistent;
		Color _background_color;

	};

}