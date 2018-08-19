#pragma once
#include "hvn3/gui2/IWidget.h"

namespace hvn3 {
	namespace Gui {

		class WidgetLayoutBuilder {

		public:
			WidgetLayoutBuilder();

			void SetPadding(float value);
			void SetInheritAnchor(bool value);

			void PlaceAt(IWidget* to_place, const PointF& position);
			void PlaceRight(IWidget* to_place);
			void PlaceRightOf(IWidget* to_place, IWidget* of);
			void PlaceLeft(IWidget* to_place);
			void PlaceLeftOf(IWidget* to_place, IWidget* of);
			void PlaceTop(IWidget* to_place);
			void PlaceTopOf(IWidget* to_place, IWidget* of);
			void PlaceBottom(IWidget* to_place);
			void PlaceBottomOf(IWidget* to_place, IWidget* of);

			void AnchorToInnerEdge(IWidget* to_place, Anchor anchors);

		private:
			float _padding;
			bool _inherit_anchor;
			IWidget* _last_widget;

			void _postPlacement(IWidget* to_place, IWidget* of);

		};

	}
}