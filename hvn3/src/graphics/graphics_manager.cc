#include "graphics/graphics_manager.h"

namespace hvn3::graphics {

	// Public members

	GraphicsManager::GraphicsManager(events::IEventManager& eventManager) :
		eventManager(eventManager) {

		eventManager.GetEventBus().Subscribe(this);

	}
	GraphicsManager::~GraphicsManager() {

	}

	// Protected members

	void GraphicsManager::OnEvent(events::DrawFrameEvent& ev) {

		eventManager.GetEventBus().Dispatch<events::DrawEvent>();

	}
	void GraphicsManager::OnEvent(events::DrawEvent& ev) {
	}

}