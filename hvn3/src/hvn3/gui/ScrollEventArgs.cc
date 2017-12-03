#include "hvn3/gui/ScrollEventArgs.h"

namespace hvn3 {

	namespace Gui {

		ScrollEventArgs::ScrollEventArgs(float percent, float position, Gui::Orientation orientation) :
			_percent(percent),
			_position(position),
			_orientation(orientation) {
		}

		float ScrollEventArgs::Percentage() const {

			return _percent;

		}
		float ScrollEventArgs::Position() const {

			return _position;

		}
		Orientation ScrollEventArgs::Orientation() const {

			return _orientation;

		}

	}

}