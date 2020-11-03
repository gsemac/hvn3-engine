#include "graphics/graphics_manager.h"

namespace hvn3::graphics {

	// Public members

	GraphicsManager::GraphicsManager(core::IDisplayManager& displayManager, events::IEventManager& eventManager) :
		displayManager(displayManager),
		eventManager(eventManager) {

		eventManager.GetEventBus().Subscribe(this);

	}
	GraphicsManager::~GraphicsManager() {

	}

	// Protected members

	void GraphicsManager::OnEvent(events::DrawFrameEvent& ev) {

		eventManager.GetEventBus().Dispatch<events::DrawEvent>(displayManager.GetDisplay().GetCanvas());

	}
	void GraphicsManager::OnEvent(events::DrawEvent& ev) {
	}

}