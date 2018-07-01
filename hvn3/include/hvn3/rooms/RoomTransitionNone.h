#pragma once
#include "hvn3/rooms/RoomTransitionBase.h"

namespace hvn3 {

	class RoomTransitionNone : public RoomTransitionBase {

	public:
		RoomTransitionNone();
		virtual ~RoomTransitionNone();

		virtual void ExitBegin() override;
		virtual bool ExitStep(UpdateEventArgs& e) override;

		virtual bool EnterStep(UpdateEventArgs& e) override;

		virtual void OnDraw(DrawEventArgs& e) override;

	};

}