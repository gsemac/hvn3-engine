#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/GuiTypeDefs.h"
#include <string>
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class DefaultWidgetRenderer : public IWidgetRenderer {

		public:
			DefaultWidgetRenderer();

			void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const override;
			SizeF MeasureString(const String& string) const override;

		protected:
			void DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget, const Color& gradient_top, const Color& gradient_bottom) const;
			void DrawButton(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawWindow(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawMenuStripItem(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawContextMenu(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawContextMenuItem(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawContextMenuItemSeparator(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawListBox(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawListBoxItem(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void DrawTextBox(Graphics::Graphics& canvas, const IWidget& widget, WidgetRenderArgs& args) const;
			void InitRenderArgs(const IWidget& widget, WidgetRenderArgs& args) const;

		private:
			Font _default_font;

			Font _createDefaultFont();
			Color _getTransitionedColor(WidgetRenderArgs& args, const std::string& id, WidgetState state, WidgetProperty prop) const;

		};

	}
}