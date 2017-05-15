#include "EventArgs.h"
#include "Control.h"

namespace Gui {

	class ScrollEventArgs : public EventArgs {

	public:
		ScrollEventArgs(float percent, float position, Orientation orientation) :
			_percent(percent),
			_position(position),
			_orientation(orientation) {
		}

		float Percentage() const;
		float Position() const;
		Orientation Orientation() const;

	private:
		float _percent;
		float _position;
		Gui::Orientation _orientation;

	};

}