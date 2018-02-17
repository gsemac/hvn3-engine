#include "hvn3/allegro/AllegroAdapter.h"
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
			_initializeStyles();
		}

		void DefaultWidgetRenderer::DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const {
			DrawButton(canvas, widget);
		}



		void DefaultWidgetRenderer::DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const {

			float x = widget.Position().X();
			float y = widget.Position().Y();
			float w = widget.Size().Width();
			float h = widget.Size().Height();

			Color outline_shadow_color(widget.Style().backColor.R() - 45, widget.Style().backColor.G() - 42, widget.Style().backColor.B() - 50); // 28, 28, 32
			Color corner_shadow_color(widget.Style().backColor.R() - 22, widget.Style().backColor.G() - 20, widget.Style().backColor.B() - 25); // 51, 50, 57

			canvas.DrawSolidRoundRectangle(x - 1.0f, y - 1.0f, w + 2.0f, h + 2.0f, outline_shadow_color, 2.0f); // outline/shadow
			canvas.DrawSolidRectangle(x, y, w, h, corner_shadow_color); // corner shadow
			al_draw_gradient_rectangle(x + 1.0f, y + 1.0f, w - 1.0f, h - 1.0f,
				System::AllegroAdapter::ToColor(widget.Style().backColor),
				al_map_rgb(68, 64, 78)); // main gradient
			canvas.DrawRoundRectangle(x, y, w - 1.0f, h - 1.0f, Color(90, 86, 103), 1.0f, 1.0f); // light outline

		}
		void DefaultWidgetRenderer::DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const {

			DrawWidgetBase(canvas, widget);

			if (widget.Style().font) {
				float text_width = widget.Text().Width(widget.Style().font);
				float text_height = widget.Text().Height(widget.Style().font);

				canvas.DrawText(
					widget.Position().X() + widget.Style().textPosition.X() + (widget.Size().Width() / 2.0f),
					widget.Position().Y() + widget.Style().textPosition.Y() + (widget.Size().Height() / 2.0f) - (text_height / 2.0f),
					widget.Text(), widget.Style().font, widget.Style().foreColor, Alignment::Center);
			}

		}



		void DefaultWidgetRenderer::_initializeStyles() {

			// The base style will serve as the base for MOST other styles (button, etc).
			WidgetStyle base_style;
			base_style.SetProperty("background-color", "linear-gradient(#494652, #44404e)");
			base_style.SetProperty("color", "#e0e0e0");
			base_style.SetProperty("font-family", "initial");

			//base_style.text.color = Color(224, 224, 224);
			//base_style.text.font = std::move(_createBaseStyleFont());
			//base_style.backgroundColor[0] = Color(73, 70, 82);
			//base_style.backgroundColor[1] = Color(68, 64, 78);

			// button:active
			//WidgetStyle button_style_active = base_style;
			//button_style_active.text.offset.y = 1.0f;

			// button:hover
			//WidgetStyle button_style_hover = base_style;
			//button_style_hover.backgroundColor[1] = base_style.backgroundColor[1].Lighter(0.5f);
			//button_style_hover.transition.duration = 0.5f;

			
			//_style_map["button:active"] = button_style_active;
			//_style_map["button:hover"] = button_style_hover;

		}
		Font DefaultWidgetRenderer::_createBaseStyleFont() {
			std::string default_font_path = System::GetSystemAssetPath(System::SystemAssetType::Fonts) + "webly.ttf";
			if (IO::File::Exists(default_font_path))
				return Font(default_font_path, 11, FontFlags::Monochrome);
			else
				return Font::BuiltIn();
		}

	}
}