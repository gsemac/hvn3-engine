#pragma once
#include "IRoomTransition.h"
#include "Color.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

	class RoomTransitionFade : public IRoomTransition, public UniqueCreateableBase<IRoomTransition, RoomTransitionFade> {

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