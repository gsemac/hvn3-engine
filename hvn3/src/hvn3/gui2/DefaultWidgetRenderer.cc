#include "hvn3/allegro/AllegroAdapter.h"
#include "hvn3/allegro/AllegroExt.h"
#include "hvn3/assets/SystemAssets.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/gui2/DefaultWidgetRenderer.h"
#include "hvn3/gui2/IWidget.h"
#include "hvn3/gui2/WidgetRenderArgs.h"
#include "hvn3/io/File.h"
#include "hvn3/utility/Utf8String.h"
#include <sstream>

namespace hvn3 {
	namespace Gui {

		static const Color DEFAULT_UPPER_GRADIENT_COLOR(73, 70, 82);
		static const Color DEFAULT_LOWER_GRADIENT_COLOR(68, 64, 78);
		static const Color DEFAULT_HIGHLIGHT_COLOR = Color::Merge(Color::Orange, DEFAULT_LOWER_GRADIENT_COLOR, 0.7f);

		PointF getEasedPoint(const WidgetRenderArgs::TransitionData<PointF>* data) {
			return PointF(data->from.x + (data->to.x - data->from.x) * data->Percentage(), data->from.y + (data->to.y - data->from.y) * data->Percentage());
		}



		DefaultWidgetRenderer::DefaultWidgetRenderer() :
			_default_font(_createDefaultFont()) {
		}
		void DefaultWidgetRenderer::DrawWidget(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			// #todo Have styles defined for each widget, and then a general style-drawing function

			std::stringstream id_reader(widget.Identifier());
			std::string id;

			while (id_reader >> id) {

				InitRenderArgs(widget, args);

				if (id == "button")
					DrawButton(canvas, widget, args);
				else if (id == "window")
					DrawWindow(canvas, widget, args);
				else if (id == "menustrip")
					DrawWidgetBase(canvas, widget, DEFAULT_UPPER_GRADIENT_COLOR, DEFAULT_UPPER_GRADIENT_COLOR);
				else if (id == "menustripitem")
					DrawMenuStripItem(canvas, widget, args);
				else if (id == "contextmenu")
					DrawContextMenu(canvas, widget, args);
				else if (id == "contextmenuitem")
					DrawContextMenuItem(canvas, widget, args);
				else if (id == "contextmenuitemseparator")
					DrawContextMenuItemSeparator(canvas, widget, args);
				else if (id == "scrollbar")
					DrawWidgetBase(canvas, widget, DEFAULT_UPPER_GRADIENT_COLOR, DEFAULT_LOWER_GRADIENT_COLOR);
				else if (id == "thumb")
					DrawWidgetBase(canvas, widget, Color::Silver, Color::Grey);
				else if (id == "tabstrip")
					DrawWidgetBase(canvas, widget, DEFAULT_UPPER_GRADIENT_COLOR, DEFAULT_LOWER_GRADIENT_COLOR);
				else if (id == "tabstripitem")
					DrawMenuStripItem(canvas, widget, args);
				else if (id == "listbox")
					DrawListBox(canvas, widget, args);
				else if (id == "listboxitem")
					DrawListBoxItem(canvas, widget, args);
				else if (id == "textbox")
					DrawTextBox(canvas, widget, args);
				else if (id == "label")
					DrawLabel(canvas, widget, args);
				else if (id == "checkbox")
					DrawCheckBox(canvas, widget, args);
				else if (_styles.count(id) > 0) {
					DrawWidgetWithStyle(canvas, widget, args, _styles.at(id));
				}

			}

		}
		SizeF DefaultWidgetRenderer::MeasureString(const String& string) const {
			return SizeF(string.Width(_default_font), string.Height(_default_font));
		}
		const Font& DefaultWidgetRenderer::GetWidgetFont(const IWidget* widget) const {
			return _default_font;
		}

		void DefaultWidgetRenderer::AddStyle(const std::string& _class, const WidgetStyle& style) {

			_styles.insert(std::make_pair(_class, style));

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
			al_draw_gradient_rectangle(x + 1.0f, y + 1.0f, w - 2.0f, h - 2.0f,
				System::AllegroAdapter::ToColor(gradient_top),
				System::AllegroAdapter::ToColor(gradient_bottom)); // main gradient
			canvas.DrawRoundRectangle(x, y, w, h, light_outline_color, 1.0f, 1.0f); // light outline

		}
		void DefaultWidgetRenderer::DrawButton(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			Color background_top_color(DEFAULT_UPPER_GRADIENT_COLOR);
			Color background_bottom_color = _getTransitionedColor(args, widget.Identifier(), widget.State(), WidgetProperty::BackgroundColor);
			auto text_offset_d = args.GetTransitionData<PointF>(WidgetProperty::TextOffset);

			DrawWidgetBase(canvas, widget, background_top_color, background_bottom_color);

			if (_default_font) {

				float text_w = widget.Text().Width(_default_font);
				float text_h = widget.Text().Height(_default_font);
				float text_x = widget.Position().X() + (widget.Size().Width() / 2.0f);
				float text_y = widget.Position().Y() + (widget.Size().Height() / 2.0f) - (text_h / 2.0f);

				if (text_offset_d != nullptr) {
					PointF text_offset = Graphics::TweenTraits<PointF>::Interpolate(text_offset_d->from, text_offset_d->to, text_offset_d->Percentage(), hvn3::Graphics::EasingFunction::Linear);
					text_x += text_offset.x;
					text_y += text_offset.y;
				}

				canvas.DrawText(text_x, text_y, widget.Text(), _default_font, Color(224, 224, 224), Alignment::Center);

			}

		}
		void DefaultWidgetRenderer::DrawWindow(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			Color background_top_color(DEFAULT_UPPER_GRADIENT_COLOR);
			Color background_bottom_color = _getTransitionedColor(args, widget.Identifier(), widget.State(), WidgetProperty::BackgroundColor);
			auto text_offset_d = args.GetTransitionData<PointF>(WidgetProperty::TextOffset);

			DrawWidgetBase(canvas, widget, background_top_color, background_bottom_color);

			if (_default_font) {

				float text_w = widget.Text().Width(_default_font);
				float text_h = widget.Text().Height(_default_font);
				float text_x = widget.Position().X() + (widget.Size().Width() / 2.0f);
				float text_y = widget.Position().Y(); // +(widget.Size().Height() / 2.0f) - (text_h / 2.0f);

				if (text_offset_d != nullptr) {
					PointF text_offset = Graphics::TweenTraits<PointF>::Interpolate(text_offset_d->from, text_offset_d->to, text_offset_d->Percentage(), hvn3::Graphics::EasingFunction::Linear);
					text_x += text_offset.x;
					text_y += text_offset.y;
				}

				canvas.DrawText(text_x, text_y, widget.Text(), _default_font, Color(224, 224, 224), Alignment::Center);

			}

		}
		void DefaultWidgetRenderer::DrawMenuStripItem(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			if (_default_font) {

				float text_x = widget.Position().X() + (widget.Size().width / 2.0f);
				float text_y = widget.Position().Y() + (widget.Size().height / 2.0f);

				if (HasFlag(widget.State(), WidgetState::Hover)) {
					canvas.DrawSolidRectangle(RectangleF(widget.Position(), widget.Size()), Color(DEFAULT_HIGHLIGHT_COLOR, 128));
					canvas.DrawRectangle(RectangleF(widget.Position(), widget.Size()), Color(DEFAULT_HIGHLIGHT_COLOR, 128), 1.0f);
				}

				canvas.DrawText(Math::Round(text_x), Math::Round(text_y), widget.Text(), _default_font, Color(224, 224, 224), Alignment::Center | Alignment::Middle);

			}

		}
		void DefaultWidgetRenderer::DrawContextMenu(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			canvas.DrawSolidRectangle(RectangleF(widget.Position(), widget.Size()), DEFAULT_UPPER_GRADIENT_COLOR);
			canvas.DrawRectangle(RectangleF(widget.Position(), widget.Size()), Color::Black, 1.0f);

			float icon_padding = 30.0f;

			canvas.DrawLine(widget.Position().x + icon_padding, widget.Position().y + 3.0f, widget.Position().x + icon_padding, widget.Position().y + widget.Height() - 3.0f, Color::LtGrey, 1.0f);

		}
		void DefaultWidgetRenderer::DrawContextMenuItem(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			if (_default_font) {

				float text_x = widget.Position().X();
				float text_y = widget.Position().Y() + (widget.Size().Height() / 2.0f);

				text_x += 30.0f + 4.0f; // icon padding + left text padding

				if (HasFlag(widget.State(), WidgetState::Hover)) {
					canvas.DrawSolidRectangle(RectangleF(widget.Position(), widget.Size()), Color(DEFAULT_HIGHLIGHT_COLOR, 128));
					canvas.DrawRectangle(RectangleF(widget.Position(), widget.Size()), Color(DEFAULT_HIGHLIGHT_COLOR, 128), 1.0f);
				}

				canvas.DrawText(Math::Round(text_x), Math::Round(text_y), widget.Text(), _default_font, Color(224, 224, 224), Alignment::Left | Alignment::Middle);

			}

		}
		void DefaultWidgetRenderer::DrawContextMenuItemSeparator(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			float y = widget.Position().y + widget.Height() / 2.0f;
			float icon_padding = 30.0f;

			canvas.DrawLine(widget.Position().x + icon_padding, y, widget.Position().x + widget.Width(), y, Color::LtGrey, 1.0f);

		}
		void DefaultWidgetRenderer::DrawListBox(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			canvas.DrawSolidRectangle(widget.Position(), widget.Size(), Color::LtGrey);
			canvas.DrawRectangle(widget.Position(), widget.Size(), Color::SlateGrey);

		}
		void DefaultWidgetRenderer::DrawListBoxItem(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			Color backcol = Color::White;

			if (HasFlag(widget.State(), WidgetState::Focus))
				backcol = Color::Blue;

			canvas.DrawSolidRectangle(widget.Position(), widget.Size(), backcol);

			if (_default_font)
				canvas.DrawText(widget.X(), widget.Y(), widget.Text(), _default_font, Color::Black);

			if (HasFlag(widget.State(), WidgetState::Selected))
				canvas.DrawRectangle(widget.Position(), widget.Size(), Color::Black, 1.0f);

		}
		void DefaultWidgetRenderer::DrawTextBox(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			canvas.DrawSolidRoundRectangle(widget.Position(), widget.Size(), Color::White, 2.0f);
			canvas.DrawRoundRectangle(widget.Position(), widget.Size(), Color::Black, 2.0f, 1.0f);

		}
		void DefaultWidgetRenderer::DrawLabel(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			canvas.DrawText(widget.X(), widget.Y(), widget.Text(), _default_font, Color::White);

		}
		void DefaultWidgetRenderer::DrawCheckBox(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const {

			// Account for the widget of the checkbox itself and the padding before the text.

			float checkbox_w = 12.0f;
			float text_left_padding = 5.0f;

			canvas.DrawText(widget.X() + checkbox_w + text_left_padding, widget.Y(), widget.Text(), _default_font, Color::White);

			// Draw the checkbox.

			float text_height = MeasureString(widget.Text()).height;
			float checkbox_x = widget.X();
			float checkbox_y = Math::Round(widget.Y() + (text_height / 2.0f) - (checkbox_w / 2.0f));

			canvas.DrawSolidRectangle(checkbox_x, checkbox_y, checkbox_w, checkbox_w, Color::White);
			canvas.DrawRectangle(checkbox_x, checkbox_y, checkbox_w, checkbox_w, Color::Black);

			float checkmark_w = 2.0f;
			float checkmark_padding = 3.0f;

			checkbox_x += checkmark_padding;
			checkbox_y += checkmark_padding;
			checkbox_w -= checkmark_padding * 2.0f;

			if (HasFlag(widget.State(), WidgetState::Checked)) {
				canvas.DrawLine(checkbox_x + checkbox_w, checkbox_y, checkbox_x + checkbox_w / 2.0f, checkbox_y + checkbox_w, Color::Black, checkmark_w);
				canvas.DrawLine(checkbox_x + checkbox_w / 2.0f, checkbox_y + checkbox_w, checkbox_x, checkbox_y + checkbox_w * 0.5f, Color::Black, checkmark_w);
			}

		}
		void DefaultWidgetRenderer::DrawWidgetWithStyle(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args, const WidgetStyle& style) const {

			auto background_image = style.GetProperty<WidgetProperty::BackgroundImage>();
			auto background_position = style.GetProperty<WidgetProperty::BackgroundPosition>();

			if (background_image != nullptr) {

				float background_position_x = widget.X();
				float background_position_y = widget.Y();

				if (background_position != nullptr) {

					if (background_position->flags == WidgetStyle::PositionFlags::None) {

						background_position_x += background_position->position.x;
						background_position_y += background_position->position.y;

					}
					else {

						if (HasFlag(background_position->flags, WidgetStyle::PositionFlags::Right))
							background_position_x += widget.Width() - background_image->Width();
						if (HasFlag(background_position->flags, WidgetStyle::PositionFlags::Bottom))
							background_position_y += widget.Height() - background_image->Height();

						if (HasFlag(background_position->flags, WidgetStyle::PositionFlags::Center)) {
							background_position_x += widget.Width() / 2.0f - background_image->Width() / 2.0f;
							background_position_y += widget.Height() / 2.0f - background_image->Height() / 2.0f;
						}

					}

				}

				canvas.DrawBitmap(background_position_x, background_position_y, *background_image);

			}

		}
		void DefaultWidgetRenderer::InitRenderArgs(const IWidget& widget, WidgetRenderArgs& args) const {

			if (args.Initialized() && args.LastState() == widget.State())
				return;

			Color default_background_color = DEFAULT_LOWER_GRADIENT_COLOR;

			// Initialize the transition data for this widget to give it a starting point.
			if (!args.Initialized()) {
				args.SetTransitionData<Color>(WidgetProperty::BackgroundColor, default_background_color, default_background_color, 0.0f);
				args.SetTransitionData<PointF>(WidgetProperty::TextOffset, PointF(0.0f, 0.0f), PointF(0.0f, 0.0f), 0.0f);
			}

			// Update the state of the args object to reflect the state of the widget.
			args.SetLastState(widget.State());

			// Update the transition data to reflect the widget's current (new) state.
			if (widget.Identifier() == "button") {
				if (HasFlag(widget.State(), WidgetState::Hover)) {
					args.SetTransitionData<Color>(WidgetProperty::BackgroundColor, Color::Merge(Color::Orange, default_background_color, 0.7f), 0.3f);
					args.SetTransitionData<PointF>(WidgetProperty::TextOffset, PointF(0.0f, 2.0f), 0.1f);
				}
				else {
					// This is the transition data when the widget changes back to its default state.
					args.SetTransitionData<Color>(WidgetProperty::BackgroundColor, default_background_color, 0.2f);
					args.SetTransitionData<PointF>(WidgetProperty::TextOffset, PointF(0.0f, 0.0f), 0.1f);
				}
			}

		}

		Font DefaultWidgetRenderer::_createDefaultFont() {
			std::string default_font_path = System::GetSystemAssetPath(System::SystemAssetType::Fonts) + "webly.ttf";
			if (IO::File::Exists(default_font_path))
				return Font(default_font_path, 12);
			else
				return Font::BuiltIn();
		}
		Color DefaultWidgetRenderer::_getTransitionedColor(WidgetRenderArgs& args, const std::string& id, WidgetState state, WidgetProperty prop) const {
			auto td_ptr = args.GetTransitionData<Color>(prop);
			if (td_ptr != nullptr)
				return Color::Merge(td_ptr->from, td_ptr->to, td_ptr->Percentage());
			return Color(68, 64, 78); // Return the default background color if we don't have one to return.
		}

	}
}