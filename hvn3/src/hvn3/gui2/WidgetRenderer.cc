#include "hvn3/gui2/WidgetRenderer.h"
#include "hvn3/allegro/AllegroExtensions.h"

namespace hvn3 {
	namespace Gui {

		void WidgetRenderer::DrawWidgetBase(Graphics::Graphics& surface, float x, float y, float width, float height) {

			// Adjust position/size values to compensate for the outline details.
			x += 1.0f;
			y += 1.0f;
			width -= 2.0f;
			height -= 2.0f;

			surface.DrawFilledRoundRectangle(x - 1.0f, y - 1.0f, width + 2.0f, height + 2.0f, Color(28, 28, 32), 2.0f); // outline/shadow
			surface.DrawFilledRectangle(x, y, width, height, Color(51, 50, 57)); // corner shadow
			al_draw_gradient_rectangle(x + 1.0f, y + 1.0f, width - 1.0f, height - 1.0f, al_map_rgb(73, 70, 82), al_map_rgb(68, 64, 78)); // main gradient			
			surface.DrawRoundRectangle(x, y, width - 1.0f, height - 1.0f, Color(90, 86, 103), 1.0f, 1.0f); // light outline

		}

	}
}