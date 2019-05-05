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

			if (auto handle = e.Context().Get<IEventManager>())
				handle->GetListenerRegistry().AddListener(this);

		}
		void OnEnd(EndEventArgs& e) override {

			if (auto handle = e.Context().Get<IEventManager>())
				handle->GetListenerRegistry().RemoveListener(this);

		}

		void OnEvent(DrawEventArgs& e) override {



		}

	private:

	};

}