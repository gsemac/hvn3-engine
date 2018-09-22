#pragma once
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/graphics/Color.h"
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

	class RoomTransitionNone : 
		public RoomTransitionBase {

	public:
		RoomTransitionNone();
		virtual ~RoomTransitionNone();

		virtual void ExitBegin() override;
		virtual bool ExitStep(UpdateEventArgs& e) override;

		virtual bool EnterStep(UpdateEventArgs& e) override;

		virtual void OnDraw(DrawEventArgs& e) override;

	};

	class RoomTransitionFade : 
		public RoomTransitionBase {

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