#pragma once
#include "hvn3/rooms/IRoomTransition.h"
#include "hvn3/graphics/Color.h"

namespace hvn3 {

	class RoomTransitionFade : public IRoomTransition {

	public:
		RoomTransitionFade();
		RoomTransitionFade(const Color& fade_to_color);
		RoomTransitionFade(const Color& fade_to_color, bool freeze_room);

		virtual void ExitBegin() override;
		virtual bool ExitStep(UpdateEventArgs& e) override;

		virtual bool EnterStep(UpdateEventArgs& e) override;

		virtual void OnDraw(DrawEventArgs& e) override;

		virtual bool AllowRoomUpdate() const override;

	private:
		bool _allow_update;
		float _alpha;
		Color _fade_to_color;

	};

}