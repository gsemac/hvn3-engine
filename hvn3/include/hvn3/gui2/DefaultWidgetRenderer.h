#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class DefaultWidgetRenderer : public IWidgetRenderer {

		public:
			DefaultWidgetRenderer();
			DefaultWidgetRenderer(const Color& default_fore_color, const Color& default_back_color, const Font& default_font);

			void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const override;

		protected:
			void DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const;
			void DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const;
			
		private:
			Color _default_fore_color;
			Color _default_back_color;
			Font _default_font;

		};

	}
}