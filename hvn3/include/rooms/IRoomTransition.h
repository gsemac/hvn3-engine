#pragma once
#include "core/UpdateEventArgs.h"
#include "core/IDrawable.h"
#include "core/UniquePolymorphicCreateableBase.h"

namespace hvn3 {

	enum class RoomTransition {
		None,
		Fade
	};

	class IRoomTransition;

	typedef std::unique_ptr<IRoomTransition> RoomTransitionPtr;

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