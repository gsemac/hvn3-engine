#pragma once

#include "core/idisplay_manager.h"
#include "events/draw_events.h"
#include "events/event_listener_base.h"
#include "events/ievent_manager.h"
#include "graphics/igraphics_manager.h"
#include "services/di_service_container.h"

namespace hvn3::graphics {

	class GraphicsManager :
		public IGraphicsManager,
		public events::EventListenerBase<events::DrawEvents> {

	public:
		HVN3_INJECT(GraphicsManager(core::IDisplayManager& displayManager, events::IEventManager& eventManager));
		~GraphicsManager();

	protected:
		void OnEvent(events::DrawFrameEvent& ev) override;
		void OnEvent(events::DrawEvent& ev) override;

	private:
		core::IDisplayManager& displayManager;
		events::IEventManager& eventManager;

	};

}