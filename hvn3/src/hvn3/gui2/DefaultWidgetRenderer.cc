#include "hvn3/graphics/Graphics.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/allegro/AllegroExtensions.h"

namespace hvn3 {
	namespace Gui {

		DefaultWidgetRenderer::DefaultWidgetRenderer(const Color& color, const Font& font) :
			_color(color),
			_font(font) {



		}

		void DefaultWidgetRenderer::DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const {

			if (widget.Name() == "button")
				DrawButton(canvas, widget);

		}



		void DefaultWidgetRenderer::DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const {

			float x = widget.Position().X();
			float y = widget.Position().Y();
			float w = widget.Size().Width();
			float h = widget.Size().Height();

			canvas.DrawSolidRoundRectangle(x - 1.0f, y - 1.0f, w + 2.0f, h + 2.0f, Color(28, 28, 32), 2.0f); // outline/shadow
			canvas.DrawSolidRectangle(x, y, w, h, Color(51, 50, 57)); // corner shadow
			al_draw_gradient_rectangle(x + 1.0f, y + 1.0f, w - 1.0f, h - 1.0f, al_map_rgb(73, 70, 82), al_map_rgb(68, 64, 78)); // main gradient
			canvas.DrawRoundRectangle(x, y, w - 1.0f, h - 1.0f, Color(90, 86, 103), 1.0f, 1.0f); // light outline

		}
		void DefaultWidgetRenderer::DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const {

			DrawWidgetBase(canvas, widget);

		}

	}
}