#pragma once

#include "core/service_provider.h"
#include "events/ievent_queue.h"
#include "events/ievent_manager.h"
#include "io/mouse_button.h"
#include "math/point.h"

#include <memory>
#include <vector>

namespace hvn3::events {

	class EventManager :
		public IEventManager {

	public:
		EventManager();

		IEventQueue& GetEventQueue() override;
		const IEventQueue& GetEventQueue() const override;
		MultiEventBus& GetEventBus() override;
		const MultiEventBus& GetEventBus() const override;

		void AddEventFilter(const EventFilterHandle& eventFilter) override;

		bool DoEvents(bool waitForEvent) override;

	private:
		std::shared_ptr<IEventQueue> eventQueue;
		MultiEventBus eventBus;
		bool pendingFrameDraw;
		std::vector<std::shared_ptr<IEventFilter>> eventFilters;

		void DoEvent(Event& ev);

		void OnDisplayClose(Event& ev);
		void OnFrameDraw();
		void OnMouseAxes(Event& ev);
		void OnTimer(Event& ev);

		math::Point2i GetMousePosition(Event& ev);
		math::Point2i GetMouseScrollOffset(Event& ev);
		io::MouseButton GetMouseButtons(Event& ev);

	};

}