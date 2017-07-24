#pragma once
#include "IRoomTransition.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

	class RoomTransitionNone : public IRoomTransition, public UniqueCreateableBase<IRoomTransition, RoomTransitionNone> {

	public:
		RoomTransitionNone();
		virtual ~RoomTransitionNone();

		virtual void ExitBegin() override;
		virtual bool ExitStep(UpdateEventArgs& e) override;

		virtual bool EnterStep(UpdateEventArgs& e) override;

		virtual void OnDraw(DrawEventArgs& e) override;

	};

}