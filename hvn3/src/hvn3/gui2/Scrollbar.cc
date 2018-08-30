#include "hvn3/gui2/Scrollbar.h"
#include "hvn3/math/MathUtils.h"

namespace hvn3 {
	namespace Gui {

		Scrollbar::Thumb::Thumb(Scrollbar* scrollbar) :
			_scrollbar(scrollbar) {

			_dragging = false;
			_offset = 0.0f;

			SetId("thumb");

		}

		void Scrollbar::Thumb::OnUpdate(WidgetUpdateEventArgs& e) {

			if (_scrollbar == nullptr)
				return;

			// Set the thumb size and position to correspond to the parent scrollbar.
			_updatePosition();
			_updateSize();

			// Update the scrollbar value in case the thumb has moved.
			if (_scrollbar->Orientation() == Orientation::Horizontal) {

				if (_scrollbar->Length() == Width())
					_scrollbar->SetValuePercent(0.0f);
				else
					_scrollbar->SetValuePercent(_offset / (_scrollbar->Length() - Width()));

			}
			else {

				if (_scrollbar->Length() == Height())
					_scrollbar->SetValuePercent(0.0f);
				else
					_scrollbar->SetValuePercent(_offset / (_scrollbar->Length() - Height()));

			}

		}
		void Scrollbar::Thumb::OnMousePressed(WidgetMousePressedEventArgs& e) {

			_dragging = true;
			_mouse_drag_offset = e.Position();
			_drag_offset = _offset;

		}
		void Scrollbar::Thumb::OnMouseReleased(WidgetMouseReleasedEventArgs& e) {
			_dragging = false;
		}
		void Scrollbar::Thumb::OnMouseMove(WidgetMouseMoveEventArgs& e) {

			if (_scrollbar == nullptr)
				return;

			if (_dragging) {

				if (_scrollbar->Orientation() == Orientation::Horizontal)
					_offset = Math::Clamp(_drag_offset + (e.Position().x - _mouse_drag_offset.x), 0.0f, _scrollbar->Width() - Width());
				else
					_offset = Math::Clamp(_drag_offset + (e.Position().y - _mouse_drag_offset.y), 0.0f, _scrollbar->Height() - Height());

			}

		}

		void Scrollbar::Thumb::_updatePosition() {

			if (_scrollbar->Orientation() == Orientation::Horizontal)
				SetPosition(_scrollbar->X() + _offset, _scrollbar->Y());
			else
				SetPosition(_scrollbar->X(), _scrollbar->Y() + _offset);

		}
		void Scrollbar::Thumb::_updateSize() {

			// The size should be such that the entire scrollbar can be scrolled as the thumb reaches the end.
			float len = Math::Max(15.0f, _scrollbar->Length() / (_scrollbar->Max() / _scrollbar->Length()));

			if (len > _scrollbar->Length())
				len = _scrollbar->Length();

			// If the current offset at this length sets the thumb beyond the length of the scrollbar, adjust the offset accordingly.

			float l_end, scrollbar_l_end;

			if (_scrollbar->Orientation() == Orientation::Horizontal) {
				l_end = _scrollbar->X() + _offset + len;
				scrollbar_l_end = _scrollbar->X() + _scrollbar->Length();
			}
			else {
				l_end = _scrollbar->Y() + _offset + len;
				scrollbar_l_end = _scrollbar->Y() + _scrollbar->Length();
			}

			if (l_end > scrollbar_l_end)
				_offset = Math::Max(0.0f, _offset - (l_end - scrollbar_l_end));

			// Update the size of the thumb.
			if (_scrollbar->Orientation() == Orientation::Horizontal)
				SetSize(len, _scrollbar->Height());
			else
				SetSize(_scrollbar->Width(), len);

		}



		Scrollbar::Scrollbar(float length, float max, Gui::Orientation orientation) :
			WidgetBase(0.0f, 0.0f, 15.0f, 15.0f) {

			_value = 0.0f;
			_goto_value = 0.0f;
			_max_scroll = max;
			_orientation = orientation;
			_thumb = nullptr;
			_smooth_scroll = false;

			SetId("scrollbar");
			SetLength(length);

		}
		float Scrollbar::Max() const {
			return _max_scroll;
		}
		void Scrollbar::SetMax(float value) {
			_max_scroll = value;
		}
		float Scrollbar::Value() const {

			if (_smooth_scroll)
				return _goto_value;
			else
				return _value;

		}
		void Scrollbar::SetValue(float value) {

			if (_smooth_scroll)
				_goto_value = value;
			else
				_value = value;

		}
		float Scrollbar::ValuePercent() const {

			if (_max_scroll == 0.0f)
				return 0.0f;

			return _value / _max_scroll;

		}
		void Scrollbar::SetValuePercent(float value) {

			value = Math::Clamp(value, 0.0f, 1.0f);

			SetValue(_max_scroll * value);

		}
		Orientation Scrollbar::Orientation() const {
			return _orientation;
		}
		float Scrollbar::Length() const {

			if (Orientation() == Orientation::Horizontal)
				return Width();
			else
				return Height();

		}
		void Scrollbar::SetLength(float value) {

			if (Orientation() == Orientation::Horizontal)
				SetWidth(value);
			else
				SetHeight(value);

		}
		bool Scrollbar::SmoothScrollEnabled() {
			return _smooth_scroll;
		}
		void Scrollbar::SetSmoothScrollEnabled(bool value) {
			_smooth_scroll = value;
		}
		void Scrollbar::SetVisible(bool value) {
			WidgetBase::SetVisible(value);

			if (_thumb != nullptr)
				_thumb->SetVisible(value);

		}
		void Scrollbar::OnManagerChanged(WidgetManagerChangedEventArgs& e) {

			// If the new manager is null, do nothing but destroy the thumb (if it exists).
			if (GetManager() == nullptr && _thumb != nullptr) {

				if (e.OldManager() != nullptr)
					e.OldManager()->Remove(_thumb);

				_thumb = nullptr;

				return;

			}

			// If the thumb already exists, move it to the new manager.
			if (_thumb != nullptr) {
				e.OldManager()->Move(_thumb, GetManager());
				return;
			}

			// If the thumb does not already exist, we need to create it and add it to the new manager.
			if (GetManager() != nullptr) {

				_thumb = new Thumb(this);

				GetManager()->Add(_thumb);
				_thumb->SetVisible(Visible());
				_thumb->BringToFront();

			}

		}
		void Scrollbar::OnZDepthChanged(WidgetZDepthChangedEventArgs& e) {

			if (_thumb != nullptr)
				_thumb->BringToFront();

		}
		void Scrollbar::OnUpdate(WidgetUpdateEventArgs& e) {

			if (_smooth_scroll && (_value != _goto_value)) {

				float inc = (_goto_value - _value) / 8.0f;
				if (Math::Abs(inc) < 1.0f)
					inc = 1.0f * Math::Sign(inc);

				_value += inc;

				if (Math::AreEqual(_value, _goto_value, 1.0f))
					_value = _goto_value;

			}

		}

	}

}