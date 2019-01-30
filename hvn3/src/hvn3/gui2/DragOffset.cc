#include "hvn3/gui2/DragOffset.h"

namespace hvn3 {
	namespace gui {

		DragOffset::DragOffset() :
			DragOffset(PointF(0.0f, 0.0f)) {
		}
		DragOffset::DragOffset(const PointF& clickedPosition) {

			SetClickedPosition(clickedPosition);

		}
		void DragOffset::SetClickedPosition(const PointF& clickedPosition) {

			_clicked_position = clickedPosition;
			_initial_offset = _current_offset;

		}
		void DragOffset::SetDraggedPosition(const PointF& draggedToPosition) {

			_current_offset = _initial_offset + (draggedToPosition - _clicked_position);

		}
		PointF DragOffset::Offset() const {

			return _current_offset;

		}
		float DragOffset::X() const {

			return Offset().x;

		}
		float DragOffset::Y() const {

			return Offset().y;

		}


	}
}