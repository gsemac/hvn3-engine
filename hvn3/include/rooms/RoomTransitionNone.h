#pragma once
#include "rooms/IRoomTransition.h"

namespace hvn3 {

	class RoomTransitionNone : public IRoomTransition {

	public:
		RoomTransitionNone();
		virtual ~RoomTransitionNone();

		virtual void ExitBegin() override;
		virtual bool ExitStep(UpdateEventArgs& e) override;

		virtual bool EnterStep(UpdateEventArgs& e) override;

		virtual void OnDraw(DrawEventArgs& e) override;

	};

}