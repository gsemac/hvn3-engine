#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class DefaultWidgetRenderer : public IWidgetRenderer {

		public:
			DefaultWidgetRenderer(const Color& color, const Font& font);

			void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const override;

		protected:
			void DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const;
			void DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const;
			
		private:
			Color _color;
			Font _font;

		};

	}
}