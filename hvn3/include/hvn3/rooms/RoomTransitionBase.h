#pragma once
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/core/IDrawable.h"
#include <memory>

namespace hvn3 {

	enum class RoomTransition {
		None,
		Fade
	};

	class RoomTransitionBase : public IDrawable {

	public:
		virtual ~RoomTransitionBase() = default;

		virtual void ExitBegin() = 0;
		virtual bool ExitStep(UpdateEventArgs& e) = 0;
		virtual void ExitEnd();

		virtual void EnterBegin();
		virtual bool EnterStep(UpdateEventArgs& e) = 0;
		virtual void EnterEnd();

		virtual void OnDraw(DrawEventArgs& e) override = 0;

		virtual bool Blocking() const;

	};

	typedef std::unique_ptr<RoomTransitionBase> RoomTransitionPtr;

}