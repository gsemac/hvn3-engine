#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class IScene {

	public:
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

	protected:
		virtual void OnEnter(EnterEventArgs& e) = 0;
		virtual void OnExit(ExitEventArgs& e) = 0;

	};

}