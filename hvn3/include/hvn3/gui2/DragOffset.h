#pragma once

#include "hvn3/math/Point2d.h"

namespace hvn3 {
	namespace gui {

		class DragOffset {

		public:
			DragOffset();
			DragOffset(const PointF& clickedPosition);

			void SetClickedPosition(const PointF& clickedPosition);
			void SetDraggedPosition(const PointF& draggedToPosition);
			PointF Offset() const;
			float X() const;
			float Y() const;

		private:
			PointF _clicked_position;
			PointF _initial_offset;
			PointF _current_offset;

		};

	}
}