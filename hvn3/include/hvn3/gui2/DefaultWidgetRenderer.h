#pragma once
#include "hvn3/graphics/Color.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/gui2/IWidgetRenderer.h"
#include "hvn3/gui2/WidgetStyle.h"
#include <string>
#include <unordered_map>

namespace hvn3 {
	namespace Gui {

		class DefaultWidgetRenderer : public IWidgetRenderer {

		public:
			DefaultWidgetRenderer();

			void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const override;

		protected:
			void DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const;
			void DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const;

		private:
			void _initializeStyles();
			Font _createBaseStyleFont();
			const WidgetStyle& _getStyleFromWidget(const IWidget& widget);

		private:
			std::unordered_map<std::string, WidgetStyle> _style_map;

		};

	}
}