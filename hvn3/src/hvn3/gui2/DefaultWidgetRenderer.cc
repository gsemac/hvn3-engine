#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/core/SystemAssets.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetRendererRenderArgs.h"
#include "hvn3/io/File.h"
#include "hvn3/utility/Utf8String.h"

namespace hvn3 {
	namespace Gui {

		DefaultWidgetRenderer::DefaultWidgetRenderer() :
			_default_font(_createDefaultFont()) {
		}

		void DefaultWidgetRenderer::DrawWidget(Graphics::Graphics& canvas, const IWidget& widget, WidgetRendererRenderArgs& args) const {

			InitRenderArgs(widget, args);

			DrawButton(canvas, widget, args);

		}



		void DefaultWidgetRenderer::DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget, const Color& gradient_top, const Color& gradient_bottom) const {

			float x = widget.Position().X();
			float y = widget.Position().Y();
			float w = widget.Size().Width();
			float h = widget.Size().Height();

			Color outline_shadow_color(28, 28, 32);
			Color corner_shadow_color(51, 50, 57);
			Color light_outline_color(90, 86, 103);

			canvas.DrawSolidRoundRectangle(x - 1.0f, y - 1.0f, w + 2.0f, h + 2.0f, outline_shadow_color, 2.0f); // outline/shadow
			canvas.DrawSolidRectangle(x, y, w, h, corner_shadow_color); // corner shadow
			al_draw_gradient_rectangle(x + 1.0f, y + 1.0f, w - 1.0f, h - 1.0f,
				System::AllegroAdapter::ToColor(gradient_top),
				System::AllegroAdapter::ToColor(gradient_bottom)); // main gradient
			canvas.DrawRoundRectangle(x, y, w - 1.0f, h - 1.0f, light_outline_color, 1.0f, 1.0f); // light outline

		}
		void DefaultWidgetRenderer::DrawButton(Graphics::Graphics& canvas, const IWidget& widget, WidgetRendererRenderArgs& args) const {

			Color background_top_color(73, 70, 82);
			Color background_bottom_color = _getTransitionedColor(args, widget.Identifier(), widget.State(), WidgetProperty::BackgroundColor);

			DrawWidgetBase(canvas, widget, background_top_color, background_bottom_color);

			if (_default_font) {
				float text_width = widget.Text().Width(_default_font);
				float text_height = widget.Text().Height(_default_font);

				canvas.DrawText(
					widget.Position().X() + (widget.Size().Width() / 2.0f),
					widget.Position().Y() + (widget.Size().Height() / 2.0f) - (text_height / 2.0f),
					widget.Text(), _default_font, Color(224, 224, 224), Alignment::Center
					);
			}

		}
		void DefaultWidgetRenderer::InitRenderArgs(const IWidget& widget, WidgetRendererRenderArgs& args) const {

			if (args.Initialized() && args.LastState() == widget.State())
				return;

			Color default_background_color(68, 64, 78);

			if (!args.Initialized())
				args.SetColorTransitionData(WidgetProperty::BackgroundColor, default_background_color, default_background_color, 0.0f);

			args.SetLastState(widget.State());

			if (widget.Identifier() == "button") {
				if (HasFlag(widget.State(), WidgetState::Hover)) {
					args.SetColorTransitionData(WidgetProperty::BackgroundColor, Color::Merge(Color::Orange, default_background_color, 0.7f), 0.3f);
					std::cout << args.GetColorTransitionData(WidgetProperty::BackgroundColor)->duration;
				}
				else
					args.SetColorTransitionData(WidgetProperty::BackgroundColor, default_background_color, 0.2f);
			}



		}



		Font DefaultWidgetRenderer::_createDefaultFont() {
			std::string default_font_path = System::GetSystemAssetPath(System::SystemAssetType::Fonts) + "webly.ttf";
			if (IO::File::Exists(default_font_path))
				return Font(default_font_path, 11, FontFlags::Monochrome);
			else
				return Font::BuiltIn();
		}
		Color DefaultWidgetRenderer::_getTransitionedColor(const WidgetRendererRenderArgs& args, const std::string& id, WidgetState state, WidgetProperty prop) const {
			auto td_ptr = args.GetColorTransitionData(prop);
			if (td_ptr != nullptr)
				return Color::Merge(td_ptr->from, td_ptr->to, td_ptr->Percentage());
			return Color(68, 64, 78);
		}
		float DefaultWidgetRenderer::_getInitialTransitionDuration(const std::string& identifier, WidgetState state) const {
			if (HasFlag(state, WidgetState::Hover))
				return 0.2f;
			return 0.1f;
		}


	}
}