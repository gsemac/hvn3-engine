#pragma once
#include "hvn3/gui2/IWidgetRenderer.h"

namespace hvn3 {
	namespace Gui {

		class DefaultWidgetRenderer : public IWidgetRenderer {

		public:
			//ButtonRenderer(const Color& )
			void DrawWidget(Graphics::Graphics& canvas, const IWidget& widget) const override;

		protected:
			void DrawWidgetBase(Graphics::Graphics& canvas, const IWidget& widget) const;
			void DrawButton(Graphics::Graphics& canvas, const IWidget& widget) const;


		private:
			
		};

	}
}