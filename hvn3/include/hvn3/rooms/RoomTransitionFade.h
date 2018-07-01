#pragma once
#include "hvn3/rooms/RoomTransitionBase.h"
#include "hvn3/graphics/Color.h"

namespace hvn3 {

	class RoomTransitionFade : public RoomTransitionBase {

	public:
		RoomTransitionFade();
		RoomTransitionFade(const Color& fade_to_color);
		RoomTransitionFade(const Color& fade_to_color, bool blocking);

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