#pragma once

#include "hvn3/graphics/Color.h"
#include "hvn3/rooms/SceneTransitionBase.h"

namespace hvn3 {

	class SceneTransitionFade :
		public SceneTransitionBase {

	public:
		SceneTransitionFade();
		SceneTransitionFade(const Color& fade_to_color);
		SceneTransitionFade(const Color& fade_to_color, bool blocking);

		virtual void ExitBegin() override;
		virtual bool ExitStep(UpdateEventArgs& e) override;

		virtual bool EnterStep(UpdateEventArgs& e) override;

		virtual void OnDraw(DrawEventArgs& e) override;

		virtual bool Blocking() const override;

	private:
		bool _blocking;
		float _alpha;
		Color _fade_to_color;

	};

}