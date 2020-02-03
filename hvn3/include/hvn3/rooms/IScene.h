#pragma once

#include "hvn3/events/EventArgs.h"
#include "hvn3/services/di_service_container.h"

namespace hvn3::scenes {

	class IScene {

	public:
		class EnterEventArgs :
			public EventArgs {

		public:
			EnterEventArgs(services::DIServiceContainer& services);

			services::DIServiceContainer& Services();

		private:
			services::DIServiceContainer* services;

		};

		class ExitEventArgs :
			public EventArgs {

		public:
			ExitEventArgs(services::DIServiceContainer& services);

			services::DIServiceContainer& Services();

		private:
			services::DIServiceContainer* services;

		};

		virtual ~IScene() = default;

		virtual void OnEnter(EnterEventArgs& e) = 0;
		virtual void OnExit(ExitEventArgs& e) = 0;

		virtual bool IsPersistent() const = 0;
		virtual void SetPersistent(bool value) = 0;

	};

}