#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/events/EventArgs.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class IScene {

	public:
		class CreateEventArgs :
			public EventArgs {

		public:
			CreateEventArgs(const ApplicationContext& context);
			ApplicationContext Context() const;

		private:
			ApplicationContext _context;

		};

		class EnterEventArgs :
			public EventArgs {

		public:
			EnterEventArgs(const ApplicationContext& context);
			ApplicationContext Context() const;

		private:
			ApplicationContext _context;

		};

		class ExitEventArgs :
			public EventArgs {

		public:
			ExitEventArgs(const ApplicationContext& context);
			ApplicationContext Context() const;

		private:
			ApplicationContext _context;

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