#pragma once

#include "hvn3/rooms/ISceneTransition.h"

namespace hvn3 {

	class SceneTransitionBase :
		public ISceneTransition {

	public:
		void ExitBegin() override;
		bool ExitStep(UpdateEventArgs& e) override;
		void ExitEnd() override;

		void EnterBegin()override;
		bool EnterStep(UpdateEventArgs& e) override;
		void EnterEnd() override;

		bool Blocking() const override;
		
		void OnDraw(DrawEventArgs& e) override;

	};

	typedef SceneTransitionBase SceneTransitionNone;

}