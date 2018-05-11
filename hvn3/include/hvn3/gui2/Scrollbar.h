#pragma once
#include "hvn3/gui2/WidgetBase.h"

namespace hvn3 {
	namespace Gui {

		class Scrollbar : public WidgetBase {

			class Thumb : public WidgetBase {

			public:
				Thumb(Scrollbar* scrollbar);

				void OnUpdate(WidgetUpdateEventArgs& e) override;
				void OnMousePressed(WidgetMousePressedEventArgs& e) override;
				void OnMouseReleased(WidgetMouseReleasedEventArgs& e) override;
				void OnMouseMove(WidgetMouseMoveEventArgs& e) override;

			private:
				Scrollbar* _scrollbar;
				float _offset;
				bool _dragging;
				PointF _mouse_drag_offset;
				float _drag_offset;

				void _updatePosition();
				void _updateSize();

			};

		public:
			Scrollbar(float length, float max, Orientation orientation);

			float Max() const;
			void SetMax(float value);
			float Value() const;
			void SetValue(float value);
			float ValuePercent() const;
			void SetValuePercent(float value);
			Orientation Orientation() const;
			float Length() const;
			void SetLength(float value);
			bool SmoothScrollEnabled();
			void SetSmoothScrollEnabled(bool value);

			void SetVisible(bool value) override;

			void OnManagerChanged(WidgetManagerChangedEventArgs& e) override;
			void OnZDepthChanged(WidgetZDepthChangedEventArgs& e) override;
			void OnUpdate(WidgetUpdateEventArgs& e) override;

		private:
			float _value;
			float _max_scroll;
			bool _smooth_scroll;
			float _goto_value;
			Gui::Orientation _orientation;
			Thumb* _thumb;

		};

	}
}