#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetStyle.h"

namespace hvn3 {
	namespace Gui {

		class DefaultWidgetRenderer : public IWidgetRenderer {

		public:
			DefaultWidgetRenderer();
			DefaultWidgetRenderer(const WidgetStyle& base_style);

			void ApplyStyleToWidget(IWidget& widget) override;
			void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const override;

		protected:
			void DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const;
			void DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const;

		private:
			void _initializeBaseStyle();
			void _initializeBaseStyleFont();

		private:
			WidgetStyle _base_style;

		};

	}
}