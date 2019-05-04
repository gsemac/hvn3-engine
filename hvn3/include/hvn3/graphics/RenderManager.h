#pragma once

#include <hvn3/core/DrawEventArgs.h>
#include <hvn3/core/ManagerBase.h>
#include <hvn3/events/EventListenerBase.h>
#include <hvn3/events/IEventManager.h>
#include <hvn3/events/EventListenerRegistry.h>

namespace hvn3 {

	class RenderManager :
		public ManagerBase<>,
		public EventListenerBase<DrawEventArgs> {

	public:
		void OnStart(StartEventArgs& e) override {

			auto event_manager = e.Context().Get<IEventManager>();

			if (event_manager)
				e.Context().Get<IEventManager>()->GetListenerRegistry().AddListener(this);

		}
		void OnEnd(EndEventArgs& e) override {

			auto event_manager = e.Context().Get<IEventManager>();

			if (event_manager)
				e.Context().Get<IEventManager>()->GetListenerRegistry().RemoveListener(this);

		}

		void OnEvent(DrawEventArgs& e) override {

			

		}

	private:

	};

}