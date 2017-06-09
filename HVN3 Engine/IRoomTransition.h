#pragma once
#include "UpdateEventArgs.h"
#include "IDrawable.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

	class IRoomTransition : public IDrawable {

	public:
		virtual ~IRoomTransition() = default;

		virtual void ExitBegin() = 0;
		virtual bool ExitStep(UpdateEventArgs& e) = 0;
		virtual void ExitEnd();

		virtual void EnterBegin();
		virtual bool EnterStep(UpdateEventArgs& e) = 0;
		virtual void EnterEnd();

		virtual void OnDraw(DrawEventArgs& e) override = 0;

		virtual bool AllowRoomUpdate() const;

	};

}