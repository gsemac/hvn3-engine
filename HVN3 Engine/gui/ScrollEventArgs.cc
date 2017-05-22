#include "ScrollEventArgs.h"

namespace hvn3 {

	namespace Gui {

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