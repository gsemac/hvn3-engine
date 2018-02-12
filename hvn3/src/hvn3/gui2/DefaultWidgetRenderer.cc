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
			_initializeBaseStyle();
		}
		DefaultWidgetRenderer::DefaultWidgetRenderer(const WidgetStyle& base_style) :
			_base_style(base_style) {
			if (!_base_style.font)
				_initializeBaseStyleFont();
		}

		void DefaultWidgetRenderer::ApplyStyleToWidget(IWidget& widget) {
			if (widget.Name() == "button")
				widget.SetStyle(_base_style);
		}
		void DefaultWidgetRenderer::DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const {
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

			if (widget.Style().font) {
				float text_width = widget.Text().Width(widget.Style().font);
				float text_height = widget.Text().Height(widget.Style().font);

				canvas.DrawText(
					widget.Position().X() + (widget.Size().Width() / 2.0f),
					widget.Position().Y() + (widget.Size().Height() / 2.0f) - (text_height / 2.0f),
					widget.Text(), widget.Style().font, widget.Style().foreColor, Alignment::Center);
			}

		}



		void DefaultWidgetRenderer::_initializeBaseStyle() {
			_base_style.foreColor = Color(224, 224, 224);
			_base_style.backColor = Color(73, 70, 82);
			_initializeBaseStyleFont();
		}
		void DefaultWidgetRenderer::_initializeBaseStyleFont() {
			if (!_base_style.font) {
				std::string default_font_path = System::GetSystemAssetPath(System::SystemAssetType::Fonts) + "webly.ttf";
				if (IO::File::Exists(default_font_path))
					_base_style.font = Font(default_font_path, 11, FontFlags::Monochrome);
				else
					_base_style.font = Font::BuiltIn();
			}
		}

	}
}