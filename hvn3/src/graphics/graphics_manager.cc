#include "graphics/color.h"
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

		core::IDisplay& display = displayManager.GetDisplay();

		if (display) {

			IGraphics& canvas = display.GetCanvas();

			canvas.Clear(Color::Silver);

			eventManager.GetEventBus().Dispatch<events::DrawEvent>(canvas);

			display.Refresh();

		}

	}
	void GraphicsManager::OnEvent(events::DrawEvent& ev) {
	}

}