#include "hvn3/gui2/WidgetLayoutBuilder.h"
#include "hvn3/gui2/WidgetManager.h"
#include "hvn3/math/MathUtils.h"
#include <cassert>

namespace hvn3 {
	namespace Gui {

		WidgetLayoutBuilder::WidgetLayoutBuilder() {

			_padding = 5.0f;
			_last_widget = nullptr;
			_inherit_anchor = true;

		}
		void WidgetLayoutBuilder::SetPadding(float value) {
			_padding = value;
		}
		void WidgetLayoutBuilder::SetInheritAnchor(bool value) {
			_inherit_anchor = value;
		}
		void WidgetLayoutBuilder::PlaceAt(IWidget* to_place, const PointF& position) {

			to_place->SetPosition(position);

			_last_widget = to_place;

		}
		void WidgetLayoutBuilder::PlaceRight(IWidget* to_place) {

			if (_last_widget == nullptr)
				return;

			PlaceRightOf(to_place, _last_widget);

		}
		void WidgetLayoutBuilder::PlaceRightOf(IWidget* to_place, IWidget* of) {

			to_place->SetPosition(of->Position().x + of->Width() + _padding, of->Position().y);

			_postPlacement(to_place, of);

		}
		void WidgetLayoutBuilder::PlaceLeft(IWidget* to_place) {

			if (_last_widget == nullptr)
				return;

			PlaceLeftOf(to_place, _last_widget);

		}
		void WidgetLayoutBuilder::PlaceLeftOf(IWidget* to_place, IWidget* of) {

			to_place->SetPosition(of->Position().x - to_place->Width() - _padding, of->Position().y);

			_postPlacement(to_place, of);

		}
		void WidgetLayoutBuilder::PlaceTop(IWidget* to_place) {

			if (_last_widget == nullptr)
				return;

			PlaceTopOf(to_place, _last_widget);

		}
		void WidgetLayoutBuilder::PlaceTopOf(IWidget* to_place, IWidget* of) {

			to_place->SetPosition(of->Position().x, of->Position().y - to_place->Height() - _padding);

			_postPlacement(to_place, of);

		}
		void WidgetLayoutBuilder::PlaceBottom(IWidget* to_place) {

			if (_last_widget == nullptr)
				return;

			PlaceBottomOf(to_place, _last_widget);

		}
		void WidgetLayoutBuilder::PlaceBottomOf(IWidget* to_place, IWidget* of) {

			to_place->SetPosition(of->Position().x, of->Position().y + of->Height() + _padding);

			_postPlacement(to_place, of);

		}
		void WidgetLayoutBuilder::AnchorToInnerEdge(IWidget* to_place, Anchor anchors) {

			if (HasFlag(anchors, Anchor::Left))
				to_place->SetPosition(PointF(0.0f, to_place->Position().y));

			if (HasFlag(anchors, Anchor::Right) && to_place->GetManager() != nullptr) {

				if (HasFlag(anchors, Anchor::Left))
					to_place->SetWidth(to_place->GetManager()->DockableRegion().Width());
				else
					to_place->SetPosition(PointF(to_place->GetManager()->DockableRegion().Width() - to_place->Width(), to_place->Position().y));

			}

			if (HasFlag(anchors, Anchor::Top))
				to_place->SetPosition(PointF(to_place->Position().x, 0.0f));

			if (HasFlag(anchors, Anchor::Bottom) && to_place->GetManager() != nullptr) {

				if (HasFlag(anchors, Anchor::Top))
					to_place->SetHeight(to_place->GetManager()->DockableRegion().Height());
				else
					to_place->SetPosition(PointF(to_place->Position().x, to_place->GetManager()->DockableRegion().Height() - to_place->Height()));

			}

			to_place->SetAnchor(anchors);

		}
		void WidgetLayoutBuilder::CenterHorizontally(std::initializer_list<IWidget*> to_place) {

			uint32_t count = 0;
			float width = 0.0f;
			WidgetManager* manager = nullptr;

			for (auto i = to_place.begin(); i != to_place.end(); ++i) {

				if (manager == nullptr) {

					manager = (*i)->GetManager();

					assert(manager != nullptr);

				}

				++count;

				width += (*i)->Width();

			}

			if (count > 0)
				width += _padding * (count - 1);

			float x = manager->DockableRegion().Width() / 2.0f - width / 2.0f;

			for (auto i = to_place.begin(); i != to_place.end(); ++i) {

				(*i)->SetPosition(Math::Round(x), (*i)->Position().y);

				x += (*i)->Width() + _padding;

			}

		}

		void WidgetLayoutBuilder::_postPlacement(IWidget* to_place, IWidget* of) {

			_last_widget = to_place;

			if (_inherit_anchor)
				to_place->SetAnchor(of->Anchor());

		}

	}

}