#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/core/SystemAssets.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/io/File.h"
#include "hvn3/utility/Utf8String.h"

namespace hvn3 {
	namespace Gui {

		DefaultWidgetRenderer::DefaultWidgetRenderer() {

			_default_fore_color = Color(224, 224, 224);
			_default_back_color = Color(73, 70, 82);

			std::string default_font_path = System::GetSystemAssetPath(System::SystemAssetType::Fonts) + "webly.ttf";
			if (IO::File::Exists(default_font_path))
				_default_font = Font(default_font_path, 11, FontFlags::Monochrome);
			else
				_default_font = Font::BuiltIn();

		}
		DefaultWidgetRenderer::DefaultWidgetRenderer(const Color& default_fore_color, const Color& default_back_color, const Font& default_font) :
			_default_fore_color(default_fore_color),
			_default_back_color(default_back_color),
			_default_font(default_font) {
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

			String text = "Hello, world!";
			float text_width = text.Width(_default_font);
			float text_height = text.Height(_default_font);

			canvas.DrawText(
				widget.Position().X() + (widget.Size().Width() / 2.0f),
				widget.Position().Y() + (widget.Size().Height() / 2.0f) - (text_height / 2.0f),
				"Hello, world!", _default_font, _default_fore_color, Alignment::Center);

		}

	}
}