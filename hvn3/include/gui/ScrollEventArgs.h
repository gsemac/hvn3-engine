#pragma once
#include "events/EventArgs.h"
#include "Control.h"

namespace hvn3 {

	namespace Gui {

		class ScrollEventArgs : public hvn3::System::EventArgs {

		public:
			ScrollEventArgs(float percent, float position, Orientation orientation);

			float Percentage() const;
			float Position() const;
			Orientation Orientation() const;

		private:
			float _percent;
			float _position;
			Gui::Orientation _orientation;

		};

	}

}