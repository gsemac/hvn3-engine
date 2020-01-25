#pragma once

#include "hvn3/events/EventArgs.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class IScene {

	public:
		class CreateEventArgs :
			public EventArgs {
		};

		class EnterEventArgs :
			public EventArgs {
		};

		class ExitEventArgs :
			public EventArgs {
		};

		virtual bool IsPersistent() const = 0;
		virtual void SetPersistent(bool value) = 0;
		virtual Color BackgroundColor() const = 0;
		virtual void SetBackgroundColor(const Color& color) = 0;

		virtual void OnCreate(CreateEventArgs& e) = 0;
		virtual void OnEnter(EnterEventArgs& e) = 0;
		virtual void OnExit(ExitEventArgs& e) = 0;

	};

}